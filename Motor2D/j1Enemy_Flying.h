#ifndef __Enemy_Flying__
#define __Enemy_Flying__

#include "j1EntityManager.h"
#include "j1Entity.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Animation.h"
#include "p2DynArray.h"
#include "j1Pathfinding.h"


struct SDL_Texture;


class j1Enemy_Flying : public j1Entity
{
public:

	j1Enemy_Flying(EntityType type);

	virtual ~j1Enemy_Flying();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	
	void Move(p2DynArray<iPoint>& path, float dt);

public:

	j1Animation flying;

	p2DynArray<iPoint>* path;

	bool path_created = false;


	fPoint position;
	SDL_Texture * tex;
	Collider* collider;
	SDL_Rect collider_aux;
};


#endif