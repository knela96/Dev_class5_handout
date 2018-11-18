#ifndef __Entity__
#define __Entity__

#include "j1EntityManager.h"
#include "j1App.h"
#include <string>
#include "p2Point.h"
#include "j1Animation.h"
#include "p2DynArray.h"
#include <list>



struct SDL_Texture;

enum class EntityType
{
	NONE = -1,
	PLAYER,
	FLYING_ENEMY,
	WALKING_ENEMY
};

class j1Entity
{
public:
	//Constructor
	j1Entity(SDL_Rect* collider_rect){}

	virtual ~j1Entity() {}

	virtual bool Awake(pugi::xml_node&) { return true; }

	virtual bool Start() { return true; }

	virtual bool PreUpdate() { return true; }

	virtual bool Update(float dt, bool do_logic) { return true; }

	virtual bool Update() { return true; }

	virtual bool PostUpdate() { return true; }

	virtual bool CleanUp() { return true; }

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) const { return true; };

	virtual void OnCollision(Collider*, Collider*) {}

public:

	p2SString name;
	p2SString texture_path;
	p2SString folder;
	EntityType type;
	fPoint position;
	SDL_Texture * graphics;
	SDL_Rect animation_Rect;
	j1Animation* current_animation;
	Collider* collider;
	bool flip;
	bool OnGround;
	bool Fall;
};

#endif //__Entity__