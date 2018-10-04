#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collisions.h"

j1Collisions::j1Collisions() : j1Module()
{
}

// Destructor
j1Collisions::~j1Collisions()
{}

// Called before render is available
bool j1Collisions::Awake(pugi::xml_node& config)
{
	LOG("Loading Collisions");
	bool ret = true;

	
	return ret;
}

void j1Collisions::Draw()
{

}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Unloading colliders");

	return true;
}

// Load new map
bool j1Collisions::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load all colliders info ----------------------------------------------
	pugi::xml_node collider;
	for (collider = map_file.child("map").child("objectgroup"); collider && ret; collider = collider.next_sibling("objectgroup"))
	{
		Colliders* col = new Colliders();

		if (ret == true)
		{
			ret = LoadColliderDetails(collider, col);
		}

		data.colliders.add(col);
	}

	return ret;
}

bool j1Collisions::LoadColliderDetails(pugi::xml_node& colliders_node, Colliders* col)
{
	bool ret = true;
	

	return ret;
}

bool j1Collisions::LoadCollider(pugi::xml_node& node, Collider* collider)
{
	bool ret = true;
	collider->width = node.attribute("width").as_uint();
	collider->height = node.attribute("height").as_uint();
	collider->position = { node.child("object").attribute("x").as_uint(),node.child("object").attribute("y").as_uint() };
	
	return ret;
}