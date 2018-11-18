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

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{
	
}

j1Entity* j1EntityManager::CreateEntity(EntityType type)
{
	j1Entity* ret = nullptr;
	switch (type) {
	case EntityType::PLAYER:
		ret = new j1Player();
		player = (j1Player*)ret;
		break;
	case EntityType::FLYING_ENEMY:		
		ret = new j1Enemy_Flying();
		break;
	case EntityType::WALKING_ENEMY:
		ret = new j1Enemy_Walking();
	}
	if (ret != nullptr)
		entities.add(ret);

	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	delete entity;
}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;

	_config = App->LoadConfig(config_file).child("entities");
	
	return true;
}

bool j1EntityManager::AwakeEntities() {
	bool ret = true;

	p2List_item<j1Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Awake(_config.child(item->data->name.GetString()));
		item = item->next;
	}

	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;

	App->collisions->setColliders();

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
	accumulated_time += dt * 1000;

	float delay = 1000 / App->frame_cap;
	
	if (accumulated_time >= delay)
		do_logic = true;

	if(dt > 0)
		UpdateAll(dt, do_logic);

	if (do_logic == true) {
		accumulated_time -= delay;
		do_logic = false;
	}

	return true;
}

bool j1EntityManager::UpdateAll(float dt, bool do_logic)
{
	bool ret = true;

	p2List_item<j1Entity*>* item;
	for (item = entities.start; item != nullptr && ret == true; item = item->next)
	{
		ret = item->data->Update(dt, do_logic);

		if (ret)
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
	p2List_item<j1Entity*>* iterator = entities.start;
	pugi::xml_node object = data.child("player");
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator && object; iterator = iterator->next)
	{
		if (iterator->data->type == EntityType::PLAYER) {
			iterator->data->Load(object);
			object = object.next_sibling("player");
		}
	}

	object = data.child("flying_enemy");
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator && object; iterator = iterator->next)
	{
		if (iterator->data->type == EntityType::FLYING_ENEMY) {
			iterator->data->Load(object);
			object = object.next_sibling("flying_enemy");
		}
	}

	object = data.child("walking_enemy");
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator && object; iterator = iterator->next)
	{
		if (iterator->data->type == EntityType::WALKING_ENEMY) {
			iterator->data->Load(object);
			object = object.next_sibling("walking_enemy");
		}
	}
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
