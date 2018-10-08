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

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* collider1, Collider* collider2);

public:
	iPoint speed;
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
private:
	p2SString			texture_path;
	p2SString			folder;
};

#endif