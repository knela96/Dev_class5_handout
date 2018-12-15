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
#include "j1SceneIntro.h"
#include "j1Window.h"
#include "j1Pathfinding.h"
#include "j1ElementGUI.h"
#include "ButtonFunctions.h"
#include "Brofiler\Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	map.create(config.child("scene1").child("map").child_value());
	cam_pos = { config.child("scene1").child("camera").attribute("x").as_int(),
				config.child("scene1").child("camera").attribute("y").as_int()
	};

	music_path.create(config.child("scene1").child("audio").child_value());


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->gui->Enable();
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
		
	debug_tex = App->tex->Load("Assets/maps/path2.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene1PreUpdate", Profiler::Color::DarkKhaki);

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

	if (App->gui->b_settings)
		CreateHUD();
	else {
		if (settings != nullptr) {
			App->gui->deleteElement(settings);
			settings = nullptr;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene1Update", Profiler::Color::DarkMagenta);
	if (App->entitymanager->GetPlayer() != nullptr) {
		if (App->entitymanager->GetPlayer()->current_life <= 0)
			App->fade->FadeToBlack(this, App->sceneintro);

		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			App->entitymanager->GetPlayer()->godmode = !App->entitymanager->GetPlayer()->godmode;

		if (App->entitymanager->GetPlayer()->win)
			App->fade->FadeToBlack(this, App->scene2);
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


	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("ScenePostUpdate", Profiler::Color::DarkOliveGreen);
	bool ret = true;


	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->audio->StopMusic();
	App->audio->UnloadFx();
	App->entitymanager->Disable();
	App->collisions->Disable();
	App->map->Disable();
	App->gui->Disable();
	settings = nullptr;
	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	load_scene = (Levels)data.attribute("value").as_uint();

	if (load_scene == Levels::Scene2) {
		this->Disable();
		App->scene2->Enable();
	}

	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data;

	player.append_attribute("value") = (int)Levels::Scene;

	return true;
}

void j1Scene::CreateHUD()
{
	if(settings == nullptr)
		settings = App->gui->AddImage({ (float)(App->render->camera.w/2) - 241, (float)(App->render->camera.h / 2) - 146 }, new SDL_Rect({ 0,0,482,293 }), Levels::Scene, windowType::SETTINGS);
}