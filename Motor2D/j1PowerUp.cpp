#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1PowerUp.h"
#include "j1Collisions.h"
#include "j1Window.h"
#include "j1Audio.h"
#include <assert.h>
#include <stdio.h>
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "Brofiler\Brofiler.h"
#include "j1EntityManager.h"

#include "SDL/include/SDL.h"


j1PowerUp::j1PowerUp(SDL_Rect* collider_rect, PowerUpTypes type) : type(type), j1Entity(collider_rect)
{
	name.create(App->entitymanager->c_powerup[(int)type].GetString());

	collider = App->collisions->AddCollider(*collider_rect, ColliderTypes::COLLIDER_POWERUP, (j1Module*)App->entitymanager);
}

j1PowerUp::~j1PowerUp()
{}

bool j1PowerUp::Awake(pugi::xml_node& config) {
	LOG("Loading PoweUp");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	texture_path.create("%s%s", folder.GetString(), config.child("texture").attribute("source").as_string());
	
	p2SString temp = App->entitymanager->c_powerup[(int)type];

	for (pugi::xml_node push_node = config.child("animations").child(temp.GetString()).child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim.loop = config.child("animations").child("coin").attribute("loop").as_bool();
	anim.speed = config.child("animations").child("coin").attribute("speed").as_float();

	current_animation = &anim;

	return ret;

}

bool j1PowerUp::Start() {

	graphics = App->tex->Load(texture_path.GetString());

	position.x = collider->rect.x;
	position.y = collider->rect.y;
	return true;

}

bool j1PowerUp::Update(float dt, bool do_logic) {

	BROFILER_CATEGORY("PowerUpUpdate1", Profiler::Color::LightSeaGreen);

	collider->rect.x = position.x;
	collider->rect.y = position.y;

	animation_Rect = current_animation->GetCurrentFrame(dt);

	return true;
}

bool j1PowerUp::Update() {

	BROFILER_CATEGORY("PowerUpUpdate2", Profiler::Color::LawnGreen);

	App->render->Blit(graphics, position.x, position.y, &animation_Rect, SDL_FLIP_NONE);

	return true;
}

bool j1PowerUp::CleanUp() {

	LOG("Unloading PowerUp");

	App->tex->UnLoad(graphics);
	graphics = nullptr;

	App->collisions->deleteCollider(collider);
	collider = nullptr;

	return true;
}

void j1PowerUp::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->gettype() == COLLIDER_PLAYER) {
		PU_Effect();

		App->collisions->deleteCollider(collider);
		collider = nullptr;
		App->entitymanager->deleteEntity(this);
	}
}

bool j1PowerUp::Load(pugi::xml_node& data) {

	type = (PowerUpTypes)data.attribute("type").as_int();

	position.x = data.child("position").attribute("x").as_uint();
	position.y = data.child("position").attribute("y").as_uint();

	return true;
}

bool j1PowerUp::Save(pugi::xml_node& data) const {

	pugi::xml_node coin = data;

	coin.append_attribute("type") = (int)type;

	coin.append_child("position").append_attribute("x") = position.x;
	coin.child("position").append_attribute("y") = position.y;
	coin.child("position").append_attribute("w") = collider->rect.w;
	coin.child("position").append_attribute("h") = collider->rect.h;

	return true;
}

void j1PowerUp::PU_Effect() {
	switch (type) {
	case COIN:
		LOG("COIN");
		break;
	case LIVES:
		LOG("LIVE");
		break;
	}
}
