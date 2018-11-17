#ifndef __Enemy_Walking__
#define __Enemy_Walking__

#include "j1EntityManager.h"
#include "j1Entity.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Animation.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"


struct SDL_Texture;


class j1Enemy_Walking : public j1Entity
{
public:

	j1Enemy_Walking();

	virtual ~j1Enemy_Walking();

	bool Awake(pugi::xml_node & config);

	bool Start();

	bool Update(float dt, bool do_logic);

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Move(const p2DynArray<iPoint>* path, float dt);

public:

	j1Animation walking;

	bool path_created = false;

	const p2DynArray<iPoint>* path;

	fPoint position;

	SDL_Texture * graphics;

	Collider* collider;

	SDL_Rect collider_aux;

private:

	SDL_Texture * debug_tex;

	iPoint origin;

	iPoint destination;

	float speed;

	p2SString			texture_path;
	p2SString			folder;
	j1Animation* current_animation = nullptr;
	SDL_Rect	animation_Rect;
	bool flip;
};


#endif