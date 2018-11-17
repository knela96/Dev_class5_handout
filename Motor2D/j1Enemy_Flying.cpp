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
#include "j1Entity.h"
#include "Brofiler\Brofiler.h"

#include "SDL/include/SDL.h"


j1Enemy_Flying::j1Enemy_Flying(EntityType type) : j1Entity(EntityType::ENEMY_FLYING)
{
	name.create("enemy_flying");
}

j1Enemy_Flying::~j1Enemy_Flying()
{}

bool j1Enemy_Flying::Start() {

	LOG("Loading enemy flying");
	tex = App->tex->Load("Assets/Sprites/Enemies/Enemies");
	return true;

}

bool j1Enemy_Flying::Update(float dt) {

	BROFILER_CATEGORY("FlyingEnemyUpdate", Profiler::Color::LightSeaGreen);
	collider->SetPos(position.x, position.y);
	// pathfinfding process
}

bool j1Enemy_Flying::CleanUp() {

	LOG("Unloading harpy");

	App->tex->UnLoad(tex);

	if (collider != nullptr)
		collider->to_delete = true;

	return true;
	}

void j1Enemy_Flying::OnCollision(Collider* col_1, Collider* col_2) {

	}

bool j1Enemy_Flying::Load(pugi::xml_node&) {

	}
bool j1Enemy_Flying::Save(pugi::xml_node&) const {

	}
	
void j1Enemy_Flying::Move(p2DynArray<iPoint>& path, float dt) {

	}