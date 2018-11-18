#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Window.h"
#include "j1Pathfinding.h"
#include "Brofiler\Brofiler.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	map.create(config.child("scene2").child("map").child_value());
	cam_pos = { config.child("scene2").child("camera").attribute("x").as_int(),
				config.child("scene2").child("camera").attribute("y").as_int()
	};

	music_path.create(config.child("scene2").child("audio").child_value());
	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	App->map->Enable();
	if (App->map->Load(map.GetString()) == true) {

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->path->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	App->collisions->Enable();
	App->entitymanager->Enable();

	App->audio->PlayMusic(music_path.GetString());

	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	BROFILER_CATEGORY("Scene2PreUpdate", Profiler::Color::RosyBrown);
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->path->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}
	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	BROFILER_CATEGORY("Scene2Update", Profiler::Color::RoyalBlue);
	if (App->entitymanager->GetPlayer() != nullptr) {
		if (App->entitymanager->GetPlayer()->current_life <= 0)
			App->fade->FadeToBlack(this, App->scene);

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			App->entitymanager->GetPlayer()->godmode = !App->entitymanager->GetPlayer()->godmode;

		if (App->entitymanager->GetPlayer()->win)
			App->fade->FadeToBlack(this, App->scene);
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->fade->FadeToBlack(this, App->scene);

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlack(this, this);

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();
	

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 100 * dt;

	App->map->Draw();
/*
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Tails Odyssey Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());*/
	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	BROFILER_CATEGORY("Scene1PostUpdate", Profiler::Color::SaddleBrown);
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");
	App->audio->StopMusic();
	App->audio->UnloadFx();
	App->entitymanager->Disable();
	App->collisions->Disable();
	App->map->Disable();
	return true;
}

bool j1Scene2::Load(pugi::xml_node& data)
{
	load_scene = (Levels)data.attribute("value").as_uint();

	if (load_scene == Scene) {
		this->Disable();
		App->scene->Enable();
	}

	return true;
}

bool j1Scene2::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data;

	player.append_attribute("value") = Scene2;

	return true;
}
