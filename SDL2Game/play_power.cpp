#include "stdafx.h"
#include "player_power.h"

PlayerPower::PlayerPower()
{
	number_ = 0;
}
PlayerPower :: ~PlayerPower()
{

}
void PlayerPower::AddPos(const int& xPos)
{
	pos_list_.push_back(xPos);
}
void PlayerPower::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("images/player_pw.png", screen);
	if (ret)
	{
		number_ = 3;
		if (pos_list_.size() > 0)
		{
			pos_list_.clear();
		}

		AddPos(20);
		AddPos(60);
		AddPos(100);
	}
	else
	{
		printf("could not load player power image\n");
	}
}
void PlayerPower::Show(SDL_Renderer* screen)
{
	for (int i = 0; i < pos_list_.size(); i++)
	{
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}
void PlayerPower::Increase()
{
	number_++;
	int last_pos = pos_list_.back();
	last_pos += 40;
	pos_list_.push_back(last_pos);
	// sau co the xu ly am thanh ow day
}
void PlayerPower::Decrease()
{
	number_--;
	pos_list_.pop_back();
}

PlayerMoney::PlayerMoney()
{
	x_pos_ = 0;
	y_pos_ = 0;
}
PlayerMoney::~PlayerMoney()
{

}
void PlayerMoney::Init(SDL_Renderer* screen)
{
	bool ret = LoadImg("images/money.png", screen);
}
void PlayerMoney::Show(SDL_Renderer* screen)
{
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}