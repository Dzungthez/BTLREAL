// Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "player_object.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"

BaseObject g_background;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH, SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gScreen = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gScreen == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gScreen,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR,
                            RENDER_DRAW_COLOR);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/5.png",gScreen);
    if (ret == false)
    {
		printf("could not load bgr");
        return false;
    }
    return true;

}

void close ()
{
    g_background.Free();
    SDL_DestroyRenderer(gScreen);
    gScreen = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
}

std::vector <ThreatsObject*> MakeThreatList()
{

	std::vector <ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left.png", gScreen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500 + i * 500);
			p_threat->set_y_pos(200); // bat dau roi tu vi tri 200

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60; // dam bao pos 1 pos 2 chua dc xpos

			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);


		}
	}

	ThreatsObject* threats_objs = new ThreatsObject[20]; // 20 con bot

	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", gScreen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);
			p_threat->set_x_pos(700 + 1200* i);
			p_threat->set_y_pos(250); // roi tu vi tri do cao 250

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int main( int argc, char* argv[] )
{
	ImpTimer fps_timer;
    if (!init())
    {
		printf("could not init %s\n", SDL_GetError());
        return -1;
    }
	if (!LoadBackground())
	{
		printf("could not load background %s\n", SDL_GetError());
		return -1;
	}
        
	GameMap game_map;
	game_map.LoadMap("map/map.dat");
	game_map.LoadTiles(gScreen);

	MainObject p_player;
	p_player.LoadImg("img/player_right.png", gScreen);
	p_player.set_clips();

	std::vector<ThreatsObject*> threats_list = MakeThreatList();
	
	bool quit = false;
	while (!quit)
    {
		fps_timer.start();
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

			p_player.HandleInputAction(e, gScreen);
        }
        SDL_SetRenderDrawColor(gScreen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);

        SDL_RenderClear(gScreen);

        g_background.Render(gScreen, NULL);


		Map map_data = game_map.getMap();

		p_player.HandleBullet(gScreen);

		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		
		p_player.DoPlayer(map_data);

		p_player.Show(gScreen);

		game_map.SetMap(map_data);
		game_map.DrawMap(gScreen);

		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(gScreen);
				p_threat->DoPlayer(map_data);
				p_threat->Show(gScreen);
			}
		}

        SDL_RenderPresent(gScreen);
		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // 1000 MILISEC
		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			SDL_Delay(delay_time); // 
		}

    }

	close();

	return 0;
}



