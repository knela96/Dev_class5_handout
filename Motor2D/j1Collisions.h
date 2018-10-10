#ifndef __j1Collisions_H__
#define __j1Collisions_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "j1Module.h"


enum ColliderTypes
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_PLAYER_SHOT,
	COLLIDER_ENEMY_SHOT,
	COLLIDER_POWERUP,
	COLLIDER_MAX,
};

struct Collider {
	SDL_Rect rect;
	bool to_delete = false;
	ColliderTypes type;
	j1Module* callback = nullptr;

	/*Collider(SDL_Rect rectangle, ColliderTypes type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}*/

	ColliderTypes gettype() { return type; }

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	iPoint AvoidCollision(iPoint speed, Collider* collider);
	iPoint CollisionSpeed(SDL_Rect* collider1, SDL_Rect* Collider, iPoint speed);
};


struct ColliderData {
	p2List<Collider*> colliders;
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

	// Called before quitting
	bool CleanUp();

	bool PreUpdate();

	bool Update(float dt);

	//bool PostUpdate();

	bool Load(pugi::xml_document& file_name);
	
	bool setColliders();

private:


public:
	ColliderData data;

private:

	bool				matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool				collisions_loaded;
	bool				debug;
};

#endif // __j1Collisions_H__