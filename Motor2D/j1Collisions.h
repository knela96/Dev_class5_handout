#ifndef __j1Collisions_H__
#define __j1Collisions_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "j1Entity.h"


enum ColliderTypes
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_WIN,
	COLLIDER_FLYING_ENEMY,
	COLLIDER_PLATFORM_ENEMY,
	COLLIDER_ENEMY_SHOT,
	COLLIDER_POWERUP,
	COLLIDER_PLAYER_SHOT,
	COLLIDER_MAX,
};

struct Spawn {
	SDL_Rect rect; 
	ColliderTypes type;
};
struct Collider {
	SDL_Rect rect;
	bool to_delete = false;
	ColliderTypes type;
	j1Module* callback = nullptr;
	bool onGround;
	bool isFalling;

	Collider();
	Collider(SDL_Rect rectangle, ColliderTypes type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	ColliderTypes gettype() { return type; }

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	//fPoint AvoidCollision(fPoint speed, Collider& collider, float dt);
	//fPoint CollisionSpeed(SDL_Rect* collider1, SDL_Rect* Collider, fPoint speed);
};


struct ColliderData {
	p2List<Collider*> colliders;
	p2List<Spawn*> info_spawns;
};



// ----------------------------------------------------
class j1Collisions : public j1Module
{
public:

	j1Collisions();

	// Destructor
	virtual ~j1Collisions();

	// Called each loop iteration
	void Draw();

	bool deleteCollider(Collider * collider);

	// Called before quitting
	bool CleanUp();

	bool PreUpdate();

	Collider * AddCollider(SDL_Rect rect, ColliderTypes type, j1Module * callback = nullptr);

	bool CheckGroundCollision(Collider * hitbox) const;

	bool Update(float dt);

	bool Load(pugi::xml_document& file_name);
	
	bool setColliders();

private:


public:
	ColliderData		data;
	bool				debug = false;

private:

	bool				matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool				collisions_loaded;
};

#endif // __j1Collisions_H__