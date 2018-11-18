#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Enemy_Flying.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1Audio.h"
#include <assert.h>
#include <stdio.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "Brofiler\Brofiler.h"

#include "SDL/include/SDL.h"


j1Enemy_Flying::j1Enemy_Flying(SDL_Rect* collider_rect) : j1Entity(collider_rect)
{
	name.create("flying_enemy");

	collider = App->collisions->AddCollider(*collider_rect, ColliderTypes::COLLIDER_FLYING_ENEMY, (j1Module*)App->entitymanager);
}

j1Enemy_Flying::~j1Enemy_Flying()
{}

bool j1Enemy_Flying::Awake(pugi::xml_node& config) {
	LOG("Loading enemy flying");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path.create("%s%s", folder.GetString(), config.child("texture").attribute("source").as_string());
	speed = config.child("walkSpeed").attribute("value").as_float();
	
	for (pugi::xml_node push_node = config.child("animations").child("walk").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		flying.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	flying.loop = config.child("animations").child("walk").attribute("loop").as_bool();
	flying.speed = config.child("animations").child("walk").attribute("speed").as_float();

	current_animation = &flying;

	path = NULL;

	return ret;

}

bool j1Enemy_Flying::Start() {
	
	graphics = App->tex->Load(texture_path.GetString());

	debug_tex = App->tex->Load("Assets/maps/path2.png");


	position.x = collider->rect.x;
	position.y = collider->rect.y;
	return true;

}

bool j1Enemy_Flying::Update(float dt,bool do_logic) {

	BROFILER_CATEGORY("FlyingEnemyUpdate1", Profiler::Color::LightSeaGreen);
	// pathfinfding process
	origin = App->map->WorldToMap(
		position.x + collider->rect.w / 2, 
		position.y + collider->rect.h / 2	);
	destination = App->map->WorldToMap(
		App->entitymanager->GetPlayer()->position.x + App->entitymanager->GetPlayer()->collider->rect.w / 2,
		App->entitymanager->GetPlayer()->position.y + App->entitymanager->GetPlayer()->collider->rect.h / 2	);
	

	if ((int)sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2)) <= 20) {
		if (do_logic) {
			target_found = App->path->CreatePath(origin, destination);
			if(target_found)
				path = App->path->GetLastPath();
			else{
				LOG("target not found");
			}
		}
		if (path !=NULL && target_found) {
			Move(path,dt);
		}
	}
	
	collider->rect.x = position.x;
	collider->rect.y = position.y;

	animation_Rect = current_animation->GetCurrentFrame(dt);
	
	return true;
}

bool j1Enemy_Flying::Update(){

	BROFILER_CATEGORY("FlyingEnemyUpdate2", Profiler::Color::LawnGreen);

	if (App->entitymanager->GetPlayer()->position.x >= position.x)
		flip = false;
	else
		flip = true;

	if (path != NULL && App->collisions->debug) {
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}
	// Draw everything --------------------------------------
	if (flip)
		App->render->Blit(graphics, position.x, position.y, &animation_Rect, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(graphics, position.x, position.y, &animation_Rect, SDL_FLIP_NONE);

	return true;
}

void j1Enemy_Flying::Move(const p2DynArray<iPoint>* path, float dt)
{
	if (path->Count() > 1) {
		iPoint pos_path1 = *path->At(0);
		iPoint pos_path2 = *path->At(1);
		if (pos_path1.x > pos_path2.x) {
			position.x -= speed * dt;
		}
		else if (pos_path1.x < pos_path2.x) {
			position.x += speed * dt;
		}

		if (pos_path1.y > pos_path2.y) {
			position.y -= speed * dt;
		}
		else if (pos_path1.y < pos_path2.y) {
			position.y += speed * dt;
		}
	}
	else {
		if (origin.x > destination.x) {
			position.x -= speed * dt;
			
		}
		else if (origin.x < destination.x) {
			position.x += speed * dt;
			
		}
	}
}

bool j1Enemy_Flying::CleanUp() {

	LOG("Unloading flying_enemy");

	App->tex->UnLoad(graphics);
	graphics = nullptr;

	App->tex->UnLoad(debug_tex);
	debug_tex = nullptr;
	collider = nullptr;

	return true;
	}

void j1Enemy_Flying::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->gettype() == COLLIDER_WALL) {
		WallCollision(collider1, collider2);
	}
	else if (collider2->gettype() == COLLIDER_PLAYER_SHOT) {
		App->collisions->deleteCollider(collider);
		collider = nullptr;
		App->entitymanager->deleteEntity(this);
	}
}

void j1Enemy_Flying::WallCollision(Collider* c1, Collider* c2)
{
	SDL_Rect collisionOverlay;
	SDL_IntersectRect(&c1->rect, &c2->rect, &collisionOverlay);

	if (collisionOverlay.w >= collisionOverlay.h) {
		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y < c2->rect.y) {	//Ground
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y--;
			}
		}
		else if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h) {	//Ceiling
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y++;
			}
		}
		position.y = c1->rect.y;
	}
	else {
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x < c2->rect.x) {	//Right
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x--;
			}

		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w > c2->rect.x + c2->rect.w) {	//Left
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x++;
			}
		}
		position.x = c1->rect.x;
	}
}

bool j1Enemy_Flying::Load(pugi::xml_node& data) {

	position.x = data.child("position").attribute("x").as_uint();

	position.y = data.child("position").attribute("y").as_uint();

	speed = data.child("walkSpeed").attribute("value").as_float();

	return true;
}
bool j1Enemy_Flying::Save(pugi::xml_node& data) const {

	pugi::xml_node enemy_flying = data;
	
	enemy_flying.append_child("position").append_attribute("x") = position.x;

	enemy_flying.child("position").append_attribute("y") = position.y;

	enemy_flying.append_child("walkSpeed").append_attribute("value") = speed;


	return true;
}

