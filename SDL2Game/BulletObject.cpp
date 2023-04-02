#include "stdafx.h"
#include "BulletObject.h"


BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SPHERE_BULLET;
}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des, int status_)
{
	bool ret = false;
	if (bullet_type_ == LASER_BULLET)
	{
		ret = LoadImg("images/laser_bullet.png", des);
	}
	else
	{
		if (status_ == 0)
			ret = LoadImg("images/sphere_bullet_right.png", des);
		else if (status_ == 1)
			ret = LoadImg("images/sphere_bullet_left.png", des);
	}
	return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border) is_move_ = false;
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0) is_move_ = false;
	}
}