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

#include "SDL/include/SDL.h"


j1Enemy_Walking::j1Enemy_Walking() : j1Entity(EntityType::WALKING_ENEMY)
{
	name.create("walking_enemy");
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

	pugi::xml_node push_node = config.child("animations").child("walk").child("frame");
	idle.PushBack({
		push_node.attribute("x").as_int(),
		push_node.attribute("y").as_int(),
		push_node.attribute("w").as_int(),
		push_node.attribute("h").as_int()
		});

	current_animation = &idle;

	return ret;

}

bool j1Enemy_Walking::Start() {

	graphics = App->tex->Load(texture_path.GetString());

	debug_tex = App->tex->Load("Assets/maps/path2.png");

	current_animation = &walking;

	position.x = collider->rect.x;
	position.y = collider->rect.y;
	return true;
}

bool j1Enemy_Walking::Update(float dt, bool do_logic) {

	// pathfinfding process
	origin = App->map->WorldToMap(
		position.x + collider->rect.w / 2,
		position.y + collider->rect.h / 2);
	destination = App->map->WorldToMap(
		App->entitymanager->player->position.x + App->entitymanager->player->collider->rect.w / 2,
		App->entitymanager->player->position.y + App->entitymanager->player->collider->rect.h / 2);
	
	OnGround = App->collisions->CheckGroundCollision(collider);
	if(!flip)
		Fall = checkPlatform({origin.x + 2,origin.y});
	else
		Fall = checkPlatform({ origin.x - 2,origin.y});
	LOG("Fall: %i", Fall);

	if ((int)sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2)) <= 20) {
		if (do_logic) {
			target_found = App->path->CreatePath(origin, destination);
			if (target_found && !Fall) {
				path = App->path->GetLastPath();
				Move(path, dt);
			}else
				App->path->ClearPath();
		}
	}else
		App->path->ClearPath();

	if (!OnGround || !Fall)
		speed.y += gravity * dt;

	position.y += speed.y * dt;

	collider->rect.x = position.x;
	collider->rect.y = position.y;

	animation_Rect = current_animation->GetCurrentFrame(dt);

	return true;
}

bool j1Enemy_Walking::Update() {
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		debug_draw = !debug_draw;
	}

	if (App->entitymanager->player->position.x >= position.x)
		flip = false;
	else
		flip = true;

	if (path != NULL && debug_draw) {
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
			position.x -= speed.x * dt;
			LOG("%f", speed.x * dt);
		}
		else if (pos_path1.x < pos_path2.x) {
			position.x += speed.x * dt;
		}
	}
	else {
		if (origin.x > destination.x) {
			position.x -= speed.x * dt;

		}
		else if (origin.x < destination.x) {
			position.x += speed.x * dt;
		}
	}
}

bool j1Enemy_Walking::CleanUp() {

	LOG("Unloading walking_enemy");

	App->tex->UnLoad(graphics);

	return true;
}

void j1Enemy_Walking::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->gettype() == 0) {
		WallCollision(collider1, collider2);
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

		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w > c2->rect.x + c2->rect.w) {	//Left
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x++;
			}
		}
		position.x = c1->rect.x;
	}
}

bool j1Enemy_Walking::checkPlatform(iPoint position) {
	bool ret = true;
	for (int i = 1; i < 10; ++i) {
		ret = App->path->IsWalkable({ position.x,position.y + i });
		if (ret == false) {
			current_animation = &walking;
			return false;
		}
	}
	current_animation = &idle;
	return ret;
}

bool j1Enemy_Walking::Load(pugi::xml_node&) {

	return true;

}
bool j1Enemy_Walking::Save(pugi::xml_node&) const {

	return true; 

}

