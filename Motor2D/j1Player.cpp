#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "j1Audio.h"
#include <assert.h>
#include <stdio.h>
#include "p2Defs.h"
#include "p2Log.h"

#include "SDL/include/SDL.h"

j1Player::j1Player()
{
	name.create("player");
	// idle animation (arcade sprite sheet)
	idle.PushBack({ 0, 0, 40, 40 });
	idle.loop = true;
	idle.speed = 0.1f;

	current_animation = &idle;
	
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading player textures");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	texture_path.create("%s%s", folder.GetString(), config.child("texture").attribute("source").as_string());

	 // arcade version
	
	//death_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-044.wav");


	return ret;
}

bool j1Player::Start() {

	camera_offset.x = App->render->camera.x;
	camera_offset.y = App->render->camera.y;

	collider->rect.w = 50;//GET VALUES FROM CONFIG_XML
	collider->rect.h = 50;

	position.x = collider->rect.x;
	position.y = collider->rect.y;

	graphics = App->tex->Load(texture_path.GetString());

	speed = { 2,2 };

	start_time = 0;
	life = 3;
	dead = false;


	return true;
}

bool j1Player::CleanUp()
{
	/*
	LOG("Unloading Player assets");
	App->textures->Unload(graphics);
	graphics = nullptr;
	App->font->UnLoad(font_score);
	font_score = -1;
	App->font->UnLoad(font_gameover);
	font_gameover = -1;
	App->audio->UnloadS(death_fx);
	death_fx = nullptr;
	App->audio->UnloadM(music);
	music = nullptr;
	*/
	return true;
}
// Update: draw background
bool j1Player::Update(float dt)
{
	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT /*|| App->input->controller[RIGHT] == KEY_STATE::KEY_REPEAT*/))
	{
		position.y -= speed.y;
	}
	if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT /*|| App->input->controller[RIGHT] == KEY_STATE::KEY_REPEAT*/))
	{
		position.y += speed.y;
	}
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT /*|| App->input->controller[RIGHT] == KEY_STATE::KEY_REPEAT*/))
	{
		position.x += speed.x;
	}
	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT /*|| App->input->controller[LEFT] == KEY_STATE::KEY_REPEAT*/))
	{
		position.x -= speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		position.x = 4700;
		position.y = 100;
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		life = 0;
		dead = true;
	}

	//Collider
	collider->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());
	

	return true;
}

void j1Player::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->type == COLLIDER_WALL) {
		if (collider1->rect.x < collider->rect.x + collider->rect.w) {
			position.x = collider->rect.x + collider->rect.w;
		}
	}	
}