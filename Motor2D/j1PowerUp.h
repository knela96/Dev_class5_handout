#ifndef __Coin__
#define __Coin__

#include "j1EntityManager.h"
#include "j1Entity.h"
#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Animation.h"
#include "p2DynArray.h"


enum PowerUpTypes
{
	NONE = -1,
	COIN,
	LIVES,
	LAST
};

class j1PowerUp : public j1Entity
{
public:

	j1PowerUp(SDL_Rect* collider_rect, PowerUpTypes type);

	~j1PowerUp();

	bool Awake(pugi::xml_node & config);

	bool Start();

	bool Update(float dt, bool do_logic);

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* col_1, Collider* col_2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void PU_Effect();

public:

	j1Animation anim;

private:
	SDL_Texture * debug_tex;
	float speed; 
	PowerUpTypes type;
};


#endif