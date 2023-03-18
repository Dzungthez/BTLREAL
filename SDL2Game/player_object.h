
#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VALUE 19
class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2
	};
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);

	// xu ly su kien
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips(); // xu ly animations
	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	void CenterEntityOnMap(Map& map_data);
	void UpdateImgPlayer(SDL_Renderer* des);
	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list_ = bullet_list;
	}

	std::vector<BulletObject*> get_bullet_list() const 
	{
		return p_bullet_list_;
	}
	void HandleBullet(SDL_Renderer* des);
	void IncreaseMoney();

private:

	int money_count;
	std::vector <BulletObject*> p_bullet_list_; // bang dan
	float x_val_; 
	float y_val_; // ko that su can thiet vi nhan vat chay tren mat dat

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8]; // 8 frames
	Input input_type_;
	int frame_; //current frame - 0 1 2....
	int status_; // left or right;
	bool on_ground;

	int map_x_; // control map position
	int map_y_;

	int come_back_time;
};



#endif // !PLAYER_OBJECT_H_
