
#ifndef PLAYER_OBJECT_H_
#define PLAYER_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"

class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_RIGHT = 0,
		WALK_LEFT = 1
	};
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);

	// xu ly su kien
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips(); // xu ly animations
private:
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


};



#endif // !PLAYER_OBJECT_H_
