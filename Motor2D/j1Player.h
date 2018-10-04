#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "j1Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update();
	bool CleanUp();

public:
	int speed = 2;
	SDL_Texture * graphics = nullptr;
	j1Animation idle;
	j1Animation* current_animation;
	iPoint position;
	iPoint camera_offset;
	Uint32 start_time;
	Uint32 aux_time;
	Collider* collider;
	bool dead = false;
	bool godmode = false;
	uint life = 3;
	char _godmode[8] = "godmode";
};

#endif