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

	// idle animation (arcade sprite sheet)
	idle.PushBack({ 97, 0, 48, 16 });
	idle.loop = true;
	idle.speed = 0.1f;

	current_animation = &idle;

}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	/*
	graphics = App->tex->Load("Assets/Sprites/Character/Player.png"); // arcade version
	
	death_fx = App->audio->LoadS("Assets/Audio Files/SFX in WAV/xmultipl-044.wav");

	current_animation = &idle;

	camera_offset.x = App->render->camera.x;
	camera_offset.y = App->render->camera.y;

	speed = 2;
	 
	nitroanim = false;

	start_time = 0;
	life = 3;
	dead = false;

	//Add a collider to the player
	collider = App->collision->AddCollider({ position.x+4, position.y+1, 22, 14 }, COLLIDER_PLAYER, this);

	powerup[BASIC_SHOOT] = true;
	powerup[BOMB_SHOOT] = false;

	font_score = App->font->Load("Assets/Sprites/UI/fonts.1.png", "0123456789í.-=éè()óòáú`´!?abcdefghijklmnopqrstuvwxyz", 2);
	font_gameover = App->font->Load("Assets/Sprites/UI/fonts.2.png", "0123456789·' ºººººººººººººabcdefghijklmnopqrstuvwxyz", 2);

	music = App->audio->LoadM("Assets/Audio Files/Music in OGG/16_Game_Over.ogg");
	*/
	return ret;
}

bool j1Player::CleanUp()
{
	/*
	position.x = 100;
	position.y = 130;

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
bool j1Player::Update()
{ 
	if ((App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT /*|| App->input->controller[RIGHT] == KEY_STATE::KEY_REPEAT*/) && position.x < App->render->camera.x / App->win->scale + App->win->width -40)
	{
		position.x += speed;
	}
	if ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT /*|| App->input->controller[LEFT] == KEY_STATE::KEY_REPEAT*/) && position.x > App->render->camera.x / App->win->scale)
	{
		position.x -= speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		position.x = 4700;
		position.y = 100;
		//App->scene_stage1->disableModules();
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		life = 0;
		dead = true;
	}

	//Collider
	//collider->SetPos(position.x+10, position.y+3);


	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());

	//collider->SetPos(position.x + 4, position.y + 1);//SET POS PLAYER_COLLIDER
	
	LOG("%i", life);
	

	return true;
}