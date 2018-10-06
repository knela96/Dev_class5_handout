#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collisions.h"

j1Collisions::j1Collisions() : j1Module()
{
}

// Destructor
j1Collisions::~j1Collisions()
{}

void j1Collisions::Draw()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;


	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		for (uint j = 0; j < data.colliders[i]->collider.count(); ++j)
		{
			if (data.colliders[i]->collider[j] == nullptr)
				continue;

			switch (data.colliders[i]->collider[j]->type)
			{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_WALL: // blue
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 0, 0, 255, alpha);
				break;
			case COLLIDER_PLAYER: // green
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 0, 255, 0, alpha);
				break;
			case COLLIDER_ENEMY: // red
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 255, 0, 0, alpha);
				break;
			case COLLIDER_PLAYER_SHOT: // yellow
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 255, 255, 0, alpha);
				break;
			case COLLIDER_ENEMY_SHOT: // magenta
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 0, 255, 255, alpha);
				break;
			case COLLIDER_POWERUP:
				App->render->DrawQuad(data.colliders[i]->collider[j]->rect, 255, 0, 255, alpha);
				break;
			}
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Unloading colliders");

	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		for (uint j = 0; j < data.colliders[i]->collider.count(); ++j)
		{
			if (data.colliders[i]->collider[j] != nullptr)
			{
				delete data.colliders[i]->collider[j];
				data.colliders[i]->collider[j] = nullptr;
			}
		}
	}

	return true;
}

bool j1Collisions::Update(float dt)
{
	Draw();
	return true;
}

// Load new map
bool j1Collisions::Load(pugi::xml_document& map_file)
{
	LOG("Loading Colliders");
	bool ret = true;

	// Load all colliders info ----------------------------------------------
	pugi::xml_node collider;
	for (collider = map_file.child("map").child("objectgroup"); collider && ret; collider = collider.next_sibling("objectgroup"))
	{
		Colliders* col = new Colliders();

		ret = LoadColliderGroup(collider, col);

		if (ret == true)
			data.colliders.add(col);
	}
	return ret;
}

bool j1Collisions::LoadColliderGroup(pugi::xml_node& node, Colliders* colliders)
{
	bool ret = true;
	colliders->type = (ColliderTypes)node.attribute("id").as_uint();
	colliders->name = node.attribute("name").as_string();

	for (pugi::xml_node object = node.child("object"); object && ret; object = object.next_sibling("object"))
	{
		Collider* col = new Collider();

		col->rect.x = object.attribute("x").as_uint();
		col->rect.y = object.attribute("y").as_uint();
		col->rect.w = object.attribute("width").as_uint();
		col->rect.h = object.attribute("height").as_uint();
		col->type = colliders->type;

		colliders->collider.add(col);
	}

	return ret;
}