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

	j1Enemy_Walking(SDL_Rect* collider_rect);

	~j1Enemy_Walking();

	bool Awake(pugi::xml_node & config);

	bool Start();

	bool Update(float dt, bool do_logic);

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	void WallCollision(Collider * c1, Collider * c2);

	bool checkPlatform(iPoint Position);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void Move(const p2DynArray<iPoint>* path, float dt);

public:

	j1Animation walking;
	j1Animation idle;

	const p2DynArray<iPoint>* path = NULL;

private:
	SDL_Texture * debug_tex;
	iPoint origin;
	iPoint destination;
	bool target_found;
	float jumpSpeed;
	float maxFallingSpeed;
	float walkSpeed;
	float gravity;
	fPoint speed;
	int w_list[13][5];
};


#endif