#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Enemy_Flying.h"
#include "j1Collisions.h"
#include "j1Enemy_Walking.h"
#include "j1Entity.h"
#include "j1Enemy_Walking.h"
#include "j1Gui.h"
#include "j1PowerUp.h"
#include "Brofiler\Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");

	//List of possible powerUpds
	c_powerup.add("coin");
	c_powerup.add("lives");

	assert(c_powerup.count() == PowerUpTypes::LAST);
}

j1EntityManager::~j1EntityManager()
{}

j1Entity* j1EntityManager::CreateEntity(EntityType type,SDL_Rect* col)
{
	j1Entity* ret = nullptr;
	switch (type) {
	case EntityType::PLAYER:
		ret = new j1Player(col);
		break;
	case EntityType::FLYING_ENEMY:		
		ret = new j1Enemy_Flying(col);
		break;
	case EntityType::WALKING_ENEMY:
		ret = new j1Enemy_Walking(col);
		break;
	case EntityType::COIN:
		ret = new j1PowerUp(col, PowerUpTypes::COIN);
		break; 
	case EntityType::LIVES:
		ret = new j1PowerUp(col, PowerUpTypes::LIVES);
		break;
	}
	if (ret != nullptr) {
		ret->type = type;
		entities.add(ret);
	}

	return ret;
}

bool j1EntityManager::deleteEntity(j1Entity* entity)
{
	if (entity != nullptr)
	{
		p2List_item<j1Entity*>* item;
		item = entities.start;

		while (item != NULL)
		{
			if (item->data == entity) {
				entities.del(item);
				break;
			}
			else
				item = item->next;
		}
	}
	return false;
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	_config = App->LoadConfig(config_file).child("entities");
	
	return true;
}

j1Player* j1EntityManager::GetPlayer()
{
	p2List_item<j1Entity*>* item = entities.start;
	while (item != nullptr)
	{
		if (item->data->type == EntityType::PLAYER)
			return (j1Player*)item->data;
		item = item->next;
	}
	return nullptr;
}

bool j1EntityManager::AwakeEntities() {
	bool ret = true;

	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Awake(_config.child(item->data->name.GetString()));
		LOG(item->data->name.GetString());
		item = item->next;
	}

	return true;
}

bool j1EntityManager::StartEntities() {
	bool ret = true;

	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	//App->collisions->setColliders();

	LOG("Added E: %i", entities.count());
	AwakeEntities();

	p2List_item<j1Entity*>* item;
	item = entities.start;
	do_logic = false;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	return true;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManagerPreUpdate", Profiler::Color::Aquamarine);

	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->PreUpdate();
		item = item->next;
	}
	return true;
}
bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManagerPostUpdate", Profiler::Color::Azure);

	bool ret = true;
	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->PostUpdate();
		item = item->next;
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManagerUpdate", Profiler::Color::Beige);

	accumulated_time += dt * 1000;

	float delay = 1000 / App->frame_cap;
	
	if (accumulated_time >= delay)
		do_logic = true;

	//if(dt > 0)
		UpdateAll(dt, do_logic);

	if (do_logic == true) {
		accumulated_time -= delay;
		do_logic = false;
	}

	return true;
}

bool j1EntityManager::UpdateAll(float dt, bool do_logic)
{
	BROFILER_CATEGORY("EntityManagerUpdateAll", Profiler::Color::Chocolate);

	bool ret = true;

	p2List_item<j1Entity*>* item;
	for (item = entities.start; item != nullptr && ret == true; item = item->next)
	{
		if(dt != 0)
			item->data->Update(dt, do_logic);

		ret = item->data->Update();
	}
	
	return ret;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* item;
	for (item = entities.end; item != NULL && ret == true; item = item->prev)
	{
		item->data->CleanUp();
		delete item->data;
		item->data = nullptr;
	}
	entities.clear();
	
	LOG("Cleaned E:%i", entities.count());
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node & data )
{
	CleanUp();
	App->gui->Start();

	j1Entity* entity;

	pugi::xml_node object = data.child("player");
	while (object) {
		int x, y, w, h;
		x = object.child("position").attribute("x").as_uint();
		y = object.child("position").attribute("y").as_uint();
		w = object.child("position").attribute("w").as_uint();
		h = object.child("position").attribute("h").as_uint();

		CreateEntity(EntityType::PLAYER, new SDL_Rect({ x,y,w,h }));
		object = object.next_sibling("player");
	}

	object = data.child("flying_enemy");
	while (object) {
		int x, y, w, h;
		x = object.child("position").attribute("x").as_uint();
		y = object.child("position").attribute("y").as_uint();
		w = object.child("position").attribute("w").as_uint();
		h = object.child("position").attribute("h").as_uint();

		CreateEntity(EntityType::FLYING_ENEMY, new SDL_Rect({ x,y,w,h }));
		object = object.next_sibling("flying_enemy");
	}

	object = data.child("walking_enemy");
	while (object) {
		int x, y, w, h;
		x = object.child("position").attribute("x").as_uint();
		y = object.child("position").attribute("y").as_uint();
		w = object.child("position").attribute("w").as_uint();
		h = object.child("position").attribute("h").as_uint();

		CreateEntity(EntityType::WALKING_ENEMY, new SDL_Rect({ x,y,w,h }));
		object = object.next_sibling("walking_enemy");
	}

	object = data.child("coin");
	while (object) {
		int x, y, w, h;
		x = object.child("position").attribute("x").as_uint();
		y = object.child("position").attribute("y").as_uint();
		w = object.child("position").attribute("w").as_uint();
		h = object.child("position").attribute("h").as_uint();

		CreateEntity(EntityType::COIN, new SDL_Rect({ x,y,w,h }));
		object = object.next_sibling("coin");
	}

	object = data.child("lives");
	while (object) {
		int x, y, w, h;
		x = object.child("position").attribute("x").as_uint();
		y = object.child("position").attribute("y").as_uint();
		w = object.child("position").attribute("w").as_uint();
		h = object.child("position").attribute("h").as_uint();

		CreateEntity(EntityType::LIVES, new SDL_Rect({ x,y,w,h }));
		object = object.next_sibling("lives");
	}


	AwakeEntities();
	StartEntities();
	
	return true;
}

bool j1EntityManager::Save(pugi::xml_node & data ) const
{

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next)
	{
			iterator->data->Save(data.append_child(iterator->data->name.GetString()));
	}
	return true;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.count(); ++i)
		if (entities[i] != nullptr && entities[i]->collider == c1)
			entities[i]->OnCollision(c1,c2);
}
