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
#include "j1Animation.h"
#include "j1SceneIntro.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Gui.h"
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

	for (pugi::xml_node push_node = config.child("sceneintro").child("animations").child("plane").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_plane.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_plane.loop = config.child("sceneintro").child("animations").child("plane").attribute("loop").as_bool();
	anim_plane.speed = config.child("sceneintro").child("animations").child("plane").attribute("speed").as_float();


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
	cameraoffset = 0;
	graphics = App->tex->Load("Assets/Sprites/Character/Player1.1.png");

	current_anim = &anim_plane;

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
	App->gui->Enable();

	App->audio->PlayMusic(music_path.GetString());

	App->gui->b_settings = false;

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

	App->input->GetMousePosition(mousepos_y.x, mousepos_y.y);

	App->map->Draw();
	App->render->camera.x = cameraoffset;
	cameraoffset -= 2;
	if (App->gui->b_settings == false) LOG("enter");
	animation_Rect = current_anim->GetCurrentFrame(dt);

	if (cameraoffset == -6112) cameraoffset = 0;

	App->render->Blit(graphics, mousepos_y.x/2-70, mousepos_y.y/2 - 20, &animation_Rect, SDL_FLIP_NONE, 2, 0.0f);

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
	App->entitymanager->Disable();
	App->collisions->Disable();
	App->map->Disable();
	App->gui->Disable();
	App->tex->UnLoad(graphics);
	settings = nullptr;
	return true;
}

bool j1SceneIntro::Load(pugi::xml_node& data)
{
	load_scene = (Levels)data.attribute("value").as_uint();

	if (load_scene == Levels::Scene) {
		this->Disable();
		App->scene->Enable();
	}else if (load_scene == Levels::Scene2) {
		this->Disable();
		App->scene2->Enable();
	}

	return true;
}

void j1SceneIntro::CreateHUD()
{
	if (!main_menu) {
		App->gui->AddButton({ (float)App->render->camera.w / 2 - 59,300 }, "START", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Start, true);
		App->gui->AddButton({ (float)App->render->camera.w / 2 - 59,360 }, "CONTINUE", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Continue, false);
		App->gui->AddButton({ (float)App->render->camera.w / 2 - 59,420 }, "SETTINGS", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Settings, true);
		App->gui->AddButton({ (float)App->render->camera.w / 2 - 59,480 }, "EXIT", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Exit, true);
		App->gui->AddButton({ (float)App->render->camera.w - 118,(float)App->render->camera.h - 62}, "CREDITS", new SDL_Rect({ 120,0,196,196 }), &App->gui->button_anim, f_Credits, true);

		App->gui->AddImage({ (float)App->render->camera.w/2 - 91,50 }, new SDL_Rect({ 0,0,182,201 }),Levels::NONE,windowType::NONE, App->gui->GetLogo());
		main_menu = true;
	}
}