#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Enemy_Walking.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1Audio.h"
#include <assert.h>
#include <stdio.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "p2Point.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "Brofiler\Brofiler.h"
#include "j1Collisions.h"

#include "SDL/include/SDL.h"


j1Enemy_Walking::j1Enemy_Walking(SDL_Rect* collider_rect) : j1Entity(collider_rect)
{
	name.create("walking_enemy");

	collider = App->collisions->AddCollider(*collider_rect, ColliderTypes::COLLIDER_PLATFORM_ENEMY, (j1Module*)App->entitymanager);
}

j1Enemy_Walking::~j1Enemy_Walking()
{}

bool j1Enemy_Walking::Awake(pugi::xml_node& config) {

	LOG("Loading enemy Walking");

	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path.create("%s%s", folder.GetString(), config.child("texture").attribute("source").as_string());
	speed.x = config.child("walkSpeed").attribute("value").as_float();

	jumpSpeed = config.child("jumpSpeed").attribute("value").as_float();
	maxFallingSpeed = config.child("maxFallingSpeed").attribute("value").as_float();
	walkSpeed = config.child("walkSpeed").attribute("value").as_float();
	gravity = config.child("gravity").attribute("value").as_float();

	for (pugi::xml_node push_node = config.child("animations").child("walk").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		walking.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	walking.loop = config.child("animations").child("walk").attribute("loop").as_bool();
	walking.speed = config.child("animations").child("walk").attribute("speed").as_float();

	pugi::xml_node push_node = config.child("animations").child("idle").child("frame");
	idle.PushBack({
		push_node.attribute("x").as_int(),
		push_node.attribute("y").as_int(),
		push_node.attribute("w").as_int(),
		push_node.attribute("h").as_int()
		});
	
	return ret;

}

bool j1Enemy_Walking::Start() {

	graphics = App->tex->Load(texture_path.GetString());

	debug_tex = App->tex->Load("Assets/maps/path2.png");

	current_animation = &idle;

	position.x = collider->rect.x;
	position.y = collider->rect.y;

	speed = { 0,0 };

	return true;
}

bool j1Enemy_Walking::Update(float dt, bool do_logic) {
	
	BROFILER_CATEGORY("WalkingEnemyUpdate1", Profiler::Color::LightSalmon);

	// pathfinfding process
	origin = App->map->WorldToMap(
		position.x + collider->rect.w / 2,
		position.y + collider->rect.h / 2);
	destination = App->map->WorldToMap(
		App->entitymanager->GetPlayer()->position.x + App->entitymanager->GetPlayer()->collider->rect.w / 2,
		App->entitymanager->GetPlayer()->position.y + App->entitymanager->GetPlayer()->collider->rect.h / 2);
	
	OnGround = App->collisions->CheckGroundCollision(collider);
	if(!flip)
		Fall = checkPlatform({origin.x + 2,origin.y});
	else
		Fall = checkPlatform({ origin.x - 2,origin.y});

	if ((int)sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2)) <= 20) {
		if (do_logic) {
			target_found = App->path->CreatePath(origin, destination);
			if (target_found && !Fall) {
				path = App->path->GetLastPath();
				if(path->Count() < 20)
					Move(path, dt);
			}
			else {
				speed.x = 0.0f;
				current_animation = &idle;
				App->path->ClearPath();
			}
		}
	}
	else {
		speed.x = 0.0f;
		App->path->ClearPath();
		current_animation = &idle;
	}

	if (!OnGround)
		speed.y += gravity * dt;

	position.x += speed.x * dt;
	position.y += speed.y * dt;

	collider->rect.x = position.x;
	collider->rect.y = position.y;

	animation_Rect = current_animation->GetCurrentFrame(dt);

	return true;
}

bool j1Enemy_Walking::Update() {

	BROFILER_CATEGORY("WalkingEnemyUpdate2", Profiler::Color::PaleTurquoise);

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

	/* //BLIT ENEMY SENSOR
	for (int i = 1; i < 10; ++i) {
		App->render->Blit(debug_tex, position.x + 2 * 16, position.y + i * 16);
		App->render->Blit(debug_tex, position.x - 2 * 16, position.y + i * 16);
	}*/

	return true;
}

void j1Enemy_Walking::Move(const p2DynArray<iPoint>* path, float dt)
{
	if (path->Count() > 1) {
		iPoint pos_path1 = *path->At(0);
		iPoint pos_path2 = *path->At(1);
		if (pos_path1.x > pos_path2.x) {
			speed.x = -walkSpeed;
		}
		else if (pos_path1.x < pos_path2.x) {
			speed.x = walkSpeed;
		}
	}
	else {
		if (origin.x > destination.x) {
			speed.x = -walkSpeed;

		}
		else if (origin.x < destination.x) {
			speed.x = walkSpeed;
		}
	}
	current_animation = &walking;
}

bool j1Enemy_Walking::CleanUp() {

	LOG("Unloading walking_enemy");
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	App->tex->UnLoad(debug_tex);
	debug_tex = nullptr;
	return true;
}

void j1Enemy_Walking::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->gettype() == 0) {
		WallCollision(collider1, collider2);
	}
	else if (collider2->gettype() == COLLIDER_PLAYER_SHOT) {
		App->collisions->deleteCollider(collider);
		collider = nullptr;
		App->entitymanager->deleteEntity(this);
	}
}

void j1Enemy_Walking::WallCollision(Collider* c1, Collider* c2)
{
	SDL_Rect collisionOverlay;
	SDL_IntersectRect(&c1->rect, &c2->rect, &collisionOverlay);

	if (collisionOverlay.w >= collisionOverlay.h) {
		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y < c2->rect.y) {	//Ground
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y--;
			}

			speed.y = 0.0f;
			OnGround = true;
		}
		else if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h) {	//Ceiling
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y++;
			}
			speed.y = 0.0f;
		}
		position.y = c1->rect.y;
	}
	else {
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x < c2->rect.x) {	//Right
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x--;
			}
			speed.x = 0.0f;
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w > c2->rect.x + c2->rect.w) {	//Left
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x++;
			}
			speed.x = 0.0f;
		}
		position.x = c1->rect.x;
	}
}

bool j1Enemy_Walking::checkPlatform(iPoint position) {
	bool ret = true;
	for (int i = 1; i < 10; ++i) {
		ret = App->path->IsWalkable({ position.x,position.y + i });
		if (ret == false) {
			return false;
		}
	}
	current_animation = &idle;
	return ret;
}

bool j1Enemy_Walking::Load(pugi::xml_node& data) {

	position.x = data.child("position").attribute("x").as_uint();

	position.y = data.child("position").attribute("y").as_uint();

	speed.x = data.child("speed").attribute("x").as_float();

	speed.y = data.child("speed").attribute("y").as_float();

	return true;
}
bool j1Enemy_Walking::Save(pugi::xml_node& data) const {

	pugi::xml_node enemy_walking = data;

	enemy_walking.append_child("position").append_attribute("x") = position.x;

	enemy_walking.child("position").append_attribute("y") = position.y;

	enemy_walking.append_child("speed").append_attribute("x") = 0;

	enemy_walking.append_child("speed").append_attribute("y") = 0;



	return true;
}