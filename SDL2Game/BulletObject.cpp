#include "stdafx.h"
#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SPHERE_BULLET;
}
BulletObject :: ~BulletObject()
{

}
void BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	if (bullet_type_ == LASER_BULLET)
	{
		LoadImg("img/laser_bullet.png", des);
	}
	else
	{
		LoadImg("img/sphere_bullet.png", des);
	}
}
void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_; // rect trong base, vi tri cua mot doi tuong hinh anh hien thi tren man hinh
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
		rect_.y -= y_val_;
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	
}