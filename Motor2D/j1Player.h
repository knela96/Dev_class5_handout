#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "j1Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

enum CharacterState
{
	Stand,
	Walk,
	Jump,
	Plane
};

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);	
	bool PostUpdate();
	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);


	bool CleanUp();

	void OnCollision(Collider* collider1, Collider* collider2);
	void setGround(bool onGround, bool isFalling);
	void cameraPos();
	void deathAnim();
	void resetPlayer();

public:
	CharacterState currentState = Stand;
	float jumpSpeed;
	float maxFallingSpeed;
	float walkSpeed;
	float gravity;
	float current_gravity;
	fPoint speed;

	bool onGround = false;
	bool isFalling = false;
	bool plane = false;
	bool b_respawn = false;

	SDL_Texture * graphics = nullptr;
	j1Animation anim_idle;
	j1Animation anim_run;
	j1Animation anim_plane;
	j1Animation anim_death;
	//j1Animation jump;
	j1Animation* current_animation = nullptr;
	iPoint position;

	Uint32 start_time;
	Uint32 aux_time;

	Collider* collider;

	bool dead = false;
	bool win = false;
	bool godmode = false;
	uint life;
	uint current_life;
	char _godmode[8] = "godmode";
private:
	p2SString			texture_path;
	p2SString			folder;
	uint				heightjump;
	iPoint				lastPosition;
	iPoint				respawn;
	bool flip;
	bool death_anim;


};

#endif