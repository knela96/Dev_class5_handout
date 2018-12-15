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
#include "j1SceneIntro.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Window.h"
#include "j1Pathfinding.h"
#include "j1ElementGUI.h"
#include "ButtonFunctions.h"
#include "Brofiler\Brofiler.h"

j1SceneIntro::j1SceneIntro() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1SceneIntro::~j1SceneIntro()
{}

// Called before render is available
bool j1SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	map.create(config.child("sceneintro").child("map").child_value());
	cam_pos = { config.child("sceneintro").child("camera").attribute("x").as_int(),
				config.child("sceneintro").child("camera").attribute("y").as_int()
	};

	music_path.create(config.child("sceneintro").child("audio").child_value());


	return ret;
}

// Called before the first frame
bool j1SceneIntro::Start()
{
	main_menu = false;

	App->gui->Enable();
	App->map->Enable();
	if (App->map->Load(map.GetString()) == true) {

		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->path->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	App->audio->PlayMusic(music_path.GetString());
	
	CreateHUD();

	return true;
}

// Called each loop iteration
bool j1SceneIntro::PreUpdate()
{
	BROFILER_CATEGORY("Scene1PreUpdate", Profiler::Color::DarkKhaki);

	return true;
}

// Called each loop iteration
bool j1SceneIntro::Update(float dt)
{
	BROFILER_CATEGORY("SceneIntroUpdate", Profiler::Color::DarkMagenta);
	
	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1SceneIntro::PostUpdate()
{
	BROFILER_CATEGORY("ScenePostUpdate", Profiler::Color::DarkOliveGreen);
	bool ret = true;


	return ret;
}

// Called before quitting
bool j1SceneIntro::CleanUp()
{
	LOG("Freeing scene");
	App->audio->StopMusic();
	App->audio->UnloadFx();
	App->map->Disable();
	App->gui->Disable();
	return true;
}

bool j1SceneIntro::Load(pugi::xml_node& data)
{
	load_scene = (Levels)data.attribute("value").as_uint();

	if (load_scene == Scene) {
		this->Disable();
		App->scene->Enable();
	}else if (load_scene == Scene2) {
		this->Disable();
		App->scene2->Enable();
	}

	return true;
}

void j1SceneIntro::CreateHUD()
{
	if (!main_menu) {
		App->gui->AddButton({ 500,200 }, "START", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Start, true);
		App->gui->AddButton({ 500,250 }, "CONTINUE", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Continue, false);
		App->gui->AddButton({ 500,300 }, "SETTINGS", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Settings, true);
		App->gui->AddButton({ 500,350 }, "CREDITS", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Credits, true);
		App->gui->AddButton({ 500,400 }, "EXIT", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Exit, true);
		main_menu = true;
	}
}