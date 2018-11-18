#ifndef __j1ENTITYMANAGER__
#define __j1ENTITYMANAGER__

#include "j1Module.h"
#include "p2List.h"

class j1Entity;
class j1Player;
class j1Enemy_Flying;
enum class EntityType;
struct SDL_Rect;

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();

	~j1EntityManager();
	
	j1Entity* CreateEntity(EntityType type, SDL_Rect* col);

	bool deleteEntity(j1Entity * collider);
	j1Player * GetPlayer();

	bool Awake(pugi::xml_node& config);
	bool AwakeEntities();
	bool Start();
	bool PreUpdate();
	bool Update(float dt); //pathfinding
	bool UpdateAll(float dt,bool do_logic); //render and collisions?
	bool PostUpdate();
	bool CleanUp();
	
	
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void OnCollision(Collider * c1, Collider * c2);


public:
	p2List<j1Entity*> entities;

private:

	float accumulated_time;
	bool do_logic;

	pugi::xml_document	config_file;
	pugi::xml_node _config;
};

#endif //__j1ENTITYMANAGER__