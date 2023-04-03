#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "player_object.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "player_power.h"
#include "Geometric.h"

TTF_Font *font_time = NULL;
TTF_Font* g_font_MENU = NULL;
BaseObject g_background;
bool init()
{
	bool success = true; //Initialization flag

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL Error: \n" << SDL_GetError();
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not enabled!";
		}

		//Create window
		gWindow = SDL_CreateWindow("platformer v1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL Error: \n" << SDL_GetError();
			success = false;
		}
		else
		{
			//Create renderer for window
			gScreen = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gScreen == NULL)
			{
				cout << "Renderer could not be created! SDL Error: \n" << SDL_GetError();
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gScreen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: \n" << IMG_GetError();
					success = false;
				}
			}
			if (TTF_Init() == -1)
			{
				success = false;
			}
		}
		font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
		g_font_MENU = TTF_OpenFont("font/dlxfont_.ttf", 80);
		if (g_font_MENU == NULL) return false; // menu font

		if (font_time == NULL)
		{
			success = false;
		}
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return false;

		// read files wav audio
		g_sound_bullet[0] = Mix_LoadWAV("sound/Gun+Silencer.wav");
		g_sound_bullet[1] = Mix_LoadWAV("sound/Gun+Shot2.wav");
		g_sound_explosion[0] = Mix_LoadWAV("sound/Gun+357+Magnum.wav");
		g_sound_jump = Mix_LoadWAV("sound/cartoon-jump-6462.wav");

		if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL || g_sound_explosion == NULL) return false;

	}

	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("images/background.png", gScreen);
	if (!ret) return false;
	return true;
}

void close()
{
	g_background.Free();
	SDL_DestroyRenderer(gScreen);
	gScreen = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
}

vector<ThreatsObject*> MakeThreatList()
{
	vector<ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[20];

	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = dynamic_threats + i;
		if (p_threat != NULL)
		{
			p_threat->LoadImg("images/thread_left.png", gScreen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);

			p_threat->set_x_pos(500 + i * 500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);

			list_threats.push_back(p_threat);
		}
	}

	ThreatsObject* threats_objs = new ThreatsObject[20];

	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = threats_objs + i;
		if (p_threat != NULL)
		{
			p_threat->LoadImg("images/threats1_left.png", gScreen);
			p_threat->set_clips();
			p_threat->set_x_pos(700 + i * 1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, gScreen);

			list_threats.push_back(p_threat);
		}
	}

	return list_threats;
}



int main(int argc, char* argv[])
{
	ImpTimer fps_timer;

	if (!init()) {
		cout << "Couldn't init \n" << SDL_GetError();
		return -1;
	}
	if (!LoadBackground()) {
		cout << "Couldn't load background \n" << SDL_GetError();
		return -1;
	}

	again_label:

	GameMap game_map;
	game_map.LoadMap("images/map.dat");
	game_map.LoadTiles(gScreen);

	MainObject p_player;
	p_player.LoadImg("images/player_right.png", gScreen);
	p_player.set_clips();

	PlayerPower player_power;
	player_power.Init(gScreen);

	PlayerMoney player_money;
	player_money.Init(gScreen);
	player_money.SetPos(SCREEN_WIDTH/2 - 300, 4);

	vector<ThreatsObject*> threats_list = MakeThreatList();

	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("images//ex1.png", gScreen);
	if (!tRet)
		{
			printf("could not load img ex1 \n");
			return -1;
		}
	exp_threat.set_clip();

	ExplosionObject exp_main;
	bool pRet = exp_main.LoadImg("images//ex3.png", gScreen);
	if (!pRet)
		{
			printf("could not load img ex3\n");
			return -1;
		}
	exp_main.set_clip();

	int num_die = 0;

	//time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;

	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);

	bool quit = false;

	int ret_menu = SDLCommonFunc::ShowMenu(gScreen, g_font_MENU, "Play Game", "Exit", "images//MENU.png");
	if (ret_menu == 1)
	{
		cout << "could not load menu img \n";
		quit = true;
	}
	while (!quit)
	{
		fps_timer.start();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			p_player.HandleInputAction(e, gScreen, g_sound_bullet, g_sound_jump);
		}

		SDL_SetRenderDrawColor(gScreen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

		SDL_RenderClear(gScreen);

		g_background.Render(gScreen, NULL);
		

		Map map_data = game_map.getMap();

		p_player.HandleBullet(gScreen);

		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(gScreen);

		game_map.SetMap(map_data);
		game_map.DrawMap(gScreen);

		// ve hinh hoc
		GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
		ColorData color_data(36, 80, 150);
		Geometric::RenderRectangle(rectangle_size, color_data, gScreen);

		GeometricFormat outlineSize(1, 1, SCREEN_WIDTH -1, 38);	

		ColorData color_data2(150, 0, 0);

		Geometric :: RenderOutline(outlineSize, color_data2, gScreen);


		player_power.Show(gScreen);
		player_money.Show(gScreen);

		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(gScreen);
				p_threat->DoPlayer(map_data);
				// threat chi ban trong tam nhin cua no
				
				p_threat->MakeBullet(gScreen, SCREEN_WIDTH, SCREEN_HEIGHT);

				p_threat->Show(gScreen);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;
				std :: vector<BulletObject*> tBuller_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBuller_list.size(); ++jj)
				{
					BulletObject* pt_bullet = tBuller_list.at(jj);
					if (pt_bullet)
					{
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1)
						{	
							//p_threat->RemoveBullet(jj);
							break;
						}
					}
				}

				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

				if (bCol1 || bCol2)
				{
					Mix_PlayChannel(-1, g_sound_explosion[0], 0);
					int width_exp_frame = exp_main.get_frame_width();
					int height_exp_frame = exp_main.get_frame_heigth();
					for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
					{
						int x_pos = (p_player.GetRect().x + p_player.get_frame_width()* 0.5) - width_exp_frame * 0.5;
						int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.Show(gScreen);
						SDL_RenderPresent(gScreen);
					}
					num_die++;

					if (num_die <= 3)
					{
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000);
						player_power.Decrease();
						player_power.Render(gScreen);
						continue;
						
					}
					else
					{
						Sleep(500);
						int ret_menu = SDLCommonFunc::ShowMenu(gScreen, g_font_MENU,
							"Play Again", "Exit",
							"img//MENU END.png");
						if (ret_menu == 1)
						{
							quit = true;
							continue;
						}
						else
						{
							quit = false;
							goto again_label;
						}
						/*p_threat->Free();
							close();
							SDL_Quit();
							return 0;*/

					
					}
					
				}

			}
		}

		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_heigth();

		std::vector <BulletObject*> bullet_arr = p_player.get_bullet_list();

		for (int r = 0; r < bullet_arr.size(); r++)
		{
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL)
			{
				for (int t = 0; t < threats_list.size(); t++)
				{
					ThreatsObject* obj_threat = threats_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect; // applied for threats
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();
						tRect.h = obj_threat->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect(); // applied for bullet (bullet has no frame clip)

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
						if (bCol)
						{
							Mix_PlayChannel(-1, g_sound_explosion[0], 0);
							mark_value += 5;
							for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
							{
								
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(gScreen);
							}
							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
						}
					}
				}
			}
		}
		// show time
		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 300 - time_val;

		if (val_time <= 0)
		{
			Sleep(500);
			int ret_menu = SDLCommonFunc::ShowMenu(gScreen, g_font_MENU,
				"Play Again", "Exit",
				"img//MENU END.png");
			if (ret_menu == 1)
			{
				quit = true;
				continue;
			}
			
		}
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, gScreen);
			time_game.RenderText(gScreen, SCREEN_WIDTH - 200, 15);
		}

		std::string val_str_mark = std::to_string(mark_value);
		std::string strMark("Mark: ");
		strMark += val_str_mark;

		mark_game.SetText(strMark);
		mark_game.LoadFromRenderText(font_time, gScreen);
		mark_game.RenderText(gScreen, SCREEN_WIDTH * 0.5 - 50, 15);

		int money_count = p_player.GetMoneyCount();
		std::string money_str = std::to_string(money_count);

		money_game.SetText(money_str);
		money_game.LoadFromRenderText(font_time, gScreen);
		money_game.RenderText(gScreen, SCREEN_WIDTH * 0.5 - 250, 15);

		

		SDL_RenderPresent(gScreen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // (ms)

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0) SDL_Delay(delay_time);
		}


	}

	for (int i = 0; i < threats_list.size(); i++)
	{
		ThreatsObject* p_threat = threats_list.at(i);
		if (p_threat != NULL)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}

	close();

	return 0;
}
