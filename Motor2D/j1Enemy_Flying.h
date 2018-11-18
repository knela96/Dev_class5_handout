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

	j1Enemy_Flying(SDL_Rect* collider_rect);

	~j1Enemy_Flying();

	bool Awake(pugi::xml_node & config);

	bool Start();

	bool Update(float dt, bool do_logic);

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	void WallCollision(Collider * c1, Collider * c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	
	void Move(const p2DynArray<iPoint>* path, float dt);

public:

	j1Animation flying;

	const p2DynArray<iPoint>* path;

private:
	SDL_Texture * debug_tex; 
	iPoint origin;
	iPoint destination;
	float speed;
	bool target_found;
	bool debug_draw = false;
	bool OnGround = true;
};


#endif