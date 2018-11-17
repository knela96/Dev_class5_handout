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


j1Enemy_Flying::j1Enemy_Flying() : j1Entity(EntityType::FLYING_ENEMY)
{
	name.create("flying_enemy");
}

j1Enemy_Flying::~j1Enemy_Flying()
{}

bool j1Enemy_Flying::Awake(pugi::xml_node& config) {
	LOG("Loading enemy flying");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path.create("%s%s", folder.GetString(), config.child("texture").attribute("source").as_string());
	
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

	return ret;

}

bool j1Enemy_Flying::Start() {
	
	graphics = App->tex->Load(texture_path.GetString());

	debug_tex = App->tex->Load("Assets/maps/path2.png");

	current_animation = &flying;

	position.x = collider->rect.x;
	position.y = collider->rect.y;
	return true;

}

bool j1Enemy_Flying::Update(float dt,bool do_logic) {

	BROFILER_CATEGORY("FlyingEnemyUpdate", Profiler::Color::LightSeaGreen);
	// pathfinfding process

	origin = App->map->WorldToMap(position.x + collider->rect.w/2, position.y + collider->rect.h / 2);
	destination = App->map->WorldToMap(App->entitymanager->player->position.x + App->entitymanager->player->collider->rect.w / 2, App->entitymanager->player->position.y + App->entitymanager->player->collider->rect.h/2);

	if ((int)sqrt(pow(destination.x - origin.x, 2) + pow(destination.y - origin.y, 2)) <= 30) {
		if (do_logic) {
			App->path->CreatePath(origin, destination);
			path = App->path->GetLastPath();
		}
		if (path != NULL) {
			Move(path,dt);
		}
	}
	
	collider->rect.x = position.x;
	collider->rect.y = position.y;

	animation_Rect = current_animation->GetCurrentFrame(dt);
	
	return true;
}

bool j1Enemy_Flying::Update(){
	if (path != NULL) {
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
	if (destination.x < origin.x) {
		position.x -= speed * dt;
		flip = true;
	}
	else if (destination.x > origin.x) {
		position.x += speed * dt;
		flip = false;
	}

	if (destination.y < origin.y) {
		position.y -= speed * dt;
	}
	else if (destination.y > origin.y) {
		position.y += speed * dt;
	}
}

bool j1Enemy_Flying::CleanUp() {

	LOG("Unloading harpy");

	App->tex->UnLoad(graphics);

	if (collider != nullptr)
		collider->to_delete = true;

	return true;
	}

void j1Enemy_Flying::OnCollision(Collider* col_1, Collider* col_2) {

}

bool j1Enemy_Flying::Load(pugi::xml_node&) {

	return true;
}
bool j1Enemy_Flying::Save(pugi::xml_node&) const {
	return true;
}

