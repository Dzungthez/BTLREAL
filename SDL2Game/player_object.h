
#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 13
#define PLAYER_JUMP_VAL 18

class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 0,
		WALK_LEFT = 1,
	};

	bool LoadImg(string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);

	void HandleInputAction(SDL_Event events, SDL_Renderer* screen, 
		Mix_Chunk* bullet_sound[2], Mix_Chunk* jump_sound);

	
	void set_clips();

	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);

	void SetMapXY(const int &map_x, const int &map_y) { map_x_ = map_x; map_y_ = map_y; }
	void CenterEntityOnMap(Map& map_data);
	void UpdateImagePlayer(SDL_Renderer* des);


	SDL_Rect GetRectFrame();


	void  set_bullet_list(vector<BulletObject*> bullet_list)
	{
		p_bullet_list_ = bullet_list;
	}
	vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }
	void HandleBullet(SDL_Renderer* des);

	void RemoveBullet(const int& idx);

	void IncreaseMoney();
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }
	void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }
	int GetMoneyCount() const { return money_count; }
	void SetMoneyCount(const int & money) { money_count = money; }
	float GetPosX() const { return x_pos_; }
	int GetMark() const { return mark_; }
	void set_mark_val(const int& mark) { mark_ = mark; }


private:

	int money_count;

	vector<BulletObject*> p_bullet_list_;
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	Input input_type_;
	int frame_;
	int status_;
	bool on_ground_;

	int map_x_;
	int map_y_;

	int come_back_time_;

	int mark_;
};



#endif // !PLAYER_OBJECT_H_
