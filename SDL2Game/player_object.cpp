#include "stdafx.h"
#include "player_object.h"

MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;

	width_frame_ = 0;
	height_frame_ = 0;
	status_ = -1;

	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
}

MainObject :: ~MainObject()
{


}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen) // load anh
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / 8; // so luong frame
		height_frame_ = rect_.h;
	}
	return ret;
}

void MainObject::set_clips() // gan 8 frame motion vao mang frame_clip[]
{
	if (width_frame_ > 0 &&height_frame_ > 0)
	{
		int x_size = 0;
		int w_size = width_frame_;
		for (int i = 0; i < 8; i++)
		{
			frame_clip_[i].x = x_size;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = w_size;
			frame_clip_[i].h = height_frame_;
			x_size += width_frame_;
			w_size += width_frame_;
			
		}
	}
}

void MainObject::Show(SDL_Renderer* des) // show chuyen dong 
{
	if (status_ == WALK_LEFT)
	{
		LoadImg("img//player_left.png", des);
	}
	else
	{
		LoadImg("img//player_right.png", des);
	}
	if (input_type_.left_ == 1 || 
			input_type_.right_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}
	if (frame_ >= 8)
	{
		frame_ = 0;
	}
	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip_[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
			{
			input_type_.right_ = 0;
			}
			break;
		case SDLK_LEFT:
			{
			input_type_.left_ = 0;
			}
			break;
		
		}
	}
}

void MainObject::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ += 0.8;
	if (y_val_ >= MAX_FALL_SPEED)
	{
		y_val_ = MAX_FALL_SPEED;
	}
	if (input_type_.left_ == 1)
	{
		x_val_ -= PLAYER_SPEED;
	}
	else if (input_type_.right_ == 1)
	{
		x_val_ += PLAYER_SPEED;
	}
	CheckToMap(map_data); // kiem tra van de va cham giua map va nhan nhan vat
}

void MainObject ::  CheckToMap(Map& map_data)
{
	int x1 = 0; //
	int x2 = 0; // gioi han kiem tra theo chieu x, tu x1-> x2

	int y1 = 0;
	int y2 = 0;

	// check horizontal
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;
	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_frame_ - 1) / TILE_SIZE;

}