#ifndef __j1Collisions_H__
#define __j1Collisions_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Collider {
	iPoint position;
	uint height;
	uint width;
};

struct Colliders {
	p2SString name;
	uint id;
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

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Load(const char* file_name);

	bool j1Collisions::LoadColliderDetails(pugi::xml_node& colliders_node, Colliders* col);
	bool j1Collisions::LoadCollider(pugi::xml_node& node, Collider* col);

private:


public:
	ColliderData data;

private:
	pugi::xml_document	map_file;
	p2SString			folder;
};

#endif // __j1Collisions_H__