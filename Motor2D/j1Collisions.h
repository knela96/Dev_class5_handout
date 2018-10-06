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

	ColliderTypes gettype() { return type; }

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

struct Colliders {
	ColliderTypes type;
	p2SString name;
	p2List<Collider*> collider;
};

struct ColliderData {
	p2List<Colliders*> colliders;
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

	//bool PreUpdate();

	bool Update(float dt);

	//bool PostUpdate();

	bool Load(pugi::xml_document& file_name);

	bool LoadColliderGroup(pugi::xml_node& node, Colliders* col);

	Collider* AddCollider(SDL_Rect rect, ColliderTypes type, j1Module* callback = nullptr);

private:


public:
	ColliderData data;

private:
	bool				collisions_loaded;
	bool				debug;
};

#endif // __j1Collisions_H__