#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Entity.h"
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
	Plane,
	Attack
};

enum CharacterFX {
	Run_fx = 1,
	Plane_fx,
	Death_fx,
	Jump_fx,
	Win_fx
};

class j1Player : public j1Entity
{
public:
	j1Player(SDL_Rect* collider_rect);
	~j1Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update();
	bool Update(float dt, bool do_logic);
	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);


	bool CleanUp();

	void OnCollision(Collider* collider1, Collider* collider2);
	void WallCollision(Collider * c1, Collider * c2);
	void setGround(bool onGround, bool isFalling);
	void cameraPos();
	void hitanim();
	void deathAnim(float dt);
	void resetPlayer();

public:
	CharacterState currentState = Stand;
	float jumpSpeed;
	float maxFallingSpeed;
	float walkSpeed;
	float gravity;
	float current_gravity;
	fPoint speed;

	bool isFalling = false;
	bool plane = false;
	bool attacked = false;
	bool b_respawn = false;

	j1Animation anim_idle;
	j1Animation anim_run;
	j1Animation anim_plane;
	j1Animation anim_death;
	j1Animation anim_jumpup;
	j1Animation anim_jumpdown;
	j1Animation anim_attack;

	Collider* attack_col;//Particle

	Uint32 start_time;
	Uint32 aux_time;
	bool dead = false;
	bool win = false;
	bool godmode = false;
	bool blink = false;
	uint life;
	uint c_blink = 0;
	uint current_life;
	char _godmode[8] = "godmode";
private:
	uint				heightjump;
	fPoint				lastPosition;
	fPoint				respawn;
	bool death_anim;
	bool hit = false;
};

#endif