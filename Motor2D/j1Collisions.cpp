#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Enemy_Walking.h"
#include "j1Enemy_Flying.h"
#include "Brofiler\Brofiler.h"

j1Collisions::j1Collisions() : j1Module()
{
	name.create("collisions");

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_FLYING_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLATFORM_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_SHOT] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FLYING_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLATFORM_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_POWERUP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_FLYING_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLATFORM_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_SHOT] = false;

	matrix[COLLIDER_WIN][COLLIDER_WALL] = true;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WIN][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WIN][COLLIDER_FLYING_ENEMY] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLATFORM_ENEMY] = false;
	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_WIN][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER_SHOT] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_FLYING_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLATFORM_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WIN] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_SHOT] = false;

	matrix[COLLIDER_POWERUP][COLLIDER_WALL] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_POWERUP][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_FLYING_ENEMY] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_PLATFORM_ENEMY] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_WIN] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_PLAYER_SHOT] = false;


	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_FLYING_ENEMY] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_PLATFORM_ENEMY] = true;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_FLYING_ENEMY][COLLIDER_PLAYER_SHOT] = true;

	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_FLYING_ENEMY] = false;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_PLATFORM_ENEMY] = false;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_WIN] = false;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_PLATFORM_ENEMY][COLLIDER_PLAYER_SHOT] = true;

	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_FLYING_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLATFORM_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WIN] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_POWERUP] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate()
{
	BROFILER_CATEGORY("CollisionsPreUpdate", Profiler::Color::DarkRed);

	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] != nullptr && data.colliders[i]->to_delete == true)
		{
			delete data.colliders[i];
			data.colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	
	Collider* c1;
	Collider* c2;


	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		int c = 0;
		// skip empty colliders
		if (data.colliders[i] == nullptr)
			continue;

		c1 = data.colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < data.colliders.count(); ++k)
		{
			// skip empty colliders
			if (data.colliders[k] == nullptr)
				continue;

			c2 = data.colliders[k];
			if (c2 != nullptr) {

				if (c1->CheckCollision(c2->rect) == true && !c1->to_delete && !c2->to_delete)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
		}

	}
	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, ColliderTypes type, j1Module* callback)
{
	Collider* ret = nullptr;

	data.colliders.add(new Collider(rect, type, callback));

	return data.colliders.end->data;
}

bool j1Collisions::CheckGroundCollision(Collider* hitbox) const
{
	bool ret = false;
	if (hitbox != nullptr) {
		Collider* c1 = hitbox;
		c1->rect.y++;
		Collider* c2;

		for (uint i = 0; i < data.colliders.count(); ++i)
		{
			Collider* nextCollider = data.colliders[i];

			if (nextCollider == nullptr)
				continue;

			if (nextCollider->type != COLLIDER_WALL)
				continue;

			if (c1->CheckCollision(nextCollider->rect) == true && !c1->to_delete && !nextCollider->to_delete) {
				if (matrix[c1->type][nextCollider->type] && c1->callback)
					ret = true;
				if (matrix[nextCollider->type][c1->type] && nextCollider->callback)
					ret = true;
			}
		}
	}
	
	return ret;
}

void j1Collisions::Draw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;


	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] == nullptr)
			continue;

		switch (data.colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(data.colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(data.colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(data.colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY:
		case COLLIDER_FLYING_ENEMY:
		case COLLIDER_PLATFORM_ENEMY:// red
			App->render->DrawQuad(data.colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_WIN: // yellow
			App->render->DrawQuad(data.colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY_SHOT: // magenta
			App->render->DrawQuad(data.colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_POWERUP:
		case COLLIDER_PLAYER_SHOT:
			App->render->DrawQuad(data.colliders[i]->rect, 255, 0, 255, alpha);
			break;
		}
	}
}

bool j1Collisions::deleteCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		for (uint i = 0; i < data.colliders.count(); ++i)
		{
			if (data.colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Unloading colliders");

	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] != nullptr)
		{
			delete data.colliders[i];
			data.colliders[i] = nullptr;
		}
	}

	data.colliders.clear();

	for (uint i = 0; i < data.info_spawns.count(); ++i)
	{
		if (data.info_spawns[i] != nullptr)
		{
			delete data.info_spawns[i];
			data.info_spawns[i] = nullptr;
		}
	}

	data.info_spawns.clear();
	
	return true;
}

bool j1Collisions::Update(float dt)
{
	BROFILER_CATEGORY("CollisionsUpdate", Profiler::Color::DarkSlateBlue);
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
	for (collider = map_file.child("map").find_child_by_attribute("name","colliders walls"); collider && ret; collider = collider.next_sibling("objectgroup").find_child_by_attribute("name", "colliders walls"))
	{
		for (pugi::xml_node object = collider.child("object"); object && ret; object = object.next_sibling("object"))
		{
			Collider* col = new Collider();
			col->rect = {
			object.attribute("x").as_int(),
			object.attribute("y").as_int(),
			object.attribute("width").as_int(),
			object.attribute("height").as_int() };
			col->type = (ColliderTypes)object.attribute("name").as_uint();

			data.colliders.add(col);
		}
	}

	for (collider = map_file.child("map").find_child_by_attribute("name", "spawn"); collider && ret; collider = collider.next_sibling("objectgroup").find_child_by_attribute("name", "spawn"))
	{
		for (pugi::xml_node object = collider.child("object"); object && ret; object = object.next_sibling("object"))
		{
			Spawn* col = new Spawn();
			col->rect.x = object.attribute("x").as_uint();
			col->rect.y = object.attribute("y").as_uint();
			col->rect.w = object.attribute("width").as_uint();
			col->rect.h = object.attribute("height").as_uint();
			col->type = (ColliderTypes)object.attribute("name").as_uint();

			data.info_spawns.add(col);
		}
	}

	ret = setColliders();

	return ret;
}


bool j1Collisions::setColliders() {
	for (uint i = 0; i < data.info_spawns.count(); ++i)
	{
		switch (data.info_spawns[i]->type) {
		case COLLIDER_PLAYER:
			App->entitymanager->CreateEntity(EntityType::PLAYER, &data.info_spawns[i]->rect);
			LOG("Added Player Entity");
			break; 
		case COLLIDER_FLYING_ENEMY:
			App->entitymanager->CreateEntity(EntityType::FLYING_ENEMY, &data.info_spawns[i]->rect);
			LOG("Added Flying Entity");
			break;
		case COLLIDER_PLATFORM_ENEMY:
			App->entitymanager->CreateEntity(EntityType::WALKING_ENEMY, &data.info_spawns[i]->rect);
			LOG("Added Walking Entity");
			break;
		}
	}
	return true;
}

Collider::Collider(){}

bool Collider::CheckCollision(const SDL_Rect & r) const
{
	if (r.y + r.h <= rect.y || r.y >= rect.y + rect.h || r.x + r.w <= rect.x || r.x >= rect.x + rect.w)
		return false;
	else
		return true;
}