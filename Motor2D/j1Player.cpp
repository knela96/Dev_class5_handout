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

	currentState = CharacterState::Jump;

	speed = { 0,0 };

	start_time = 0;
	life = 3;
	dead = false;

	jumpSpeed = 20.0f;//NODE
	maxFallingSpeed = 10.0f;//NODE
	walkSpeed = 8.0f;//NODE
	gravity = 2.0f; //NODE

	scale = { 1,1 };

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
	switch (currentState)
	{
	case CharacterState::Stand:

		if (!collider->onGround)
		{
			currentState = CharacterState::Jump;
			break;
		}

		//if left or right key is pressed, but not both
		if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
		{
			currentState = CharacterState::Walk;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			speed.y = -jumpSpeed;
			currentState = CharacterState::Jump;
			collider->onGround = false;
			isFalling = true;
			break;
		}

		break;
	case CharacterState::Walk:

		if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
		{
			currentState = CharacterState::Stand;
			speed.x = 0;
			break;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D))
		{
			speed.x = walkSpeed;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A))
		{
			speed.x = -walkSpeed;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			speed.y = -jumpSpeed;
			currentState = CharacterState::Jump;
			collider->onGround = false;
			isFalling = true;
			break;
		}
		else if (!collider->onGround)
		{
			currentState = CharacterState::Jump;
			break;
		}
		break;
	case CharacterState::Jump:

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			plane = true;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
			if (plane)
				gravity = 1.0f;
			else
				gravity = 2.0f;
		}
		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)	{
			if(speed.y < 0.0f)
				speed.y = MAX(speed.y, -2.0f);
			if (!onGround)
				plane = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
		{
			speed.x = 0.0f;
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			speed.x = walkSpeed;
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			speed.x = -walkSpeed;
		}
		
		if (collider->onGround)
		{
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				currentState = CharacterState::Stand;
				speed.x = 0;
				speed.y = 0;
				plane = false;
				isFalling = false;
			}
			else 
			{
				currentState = CharacterState::Stand;
				speed.y = 0;
				plane = false;
				isFalling = false;
			}
		}
		break;
	}

	speed.y += gravity;
	if (!plane)
		speed.y = MIN(speed.y, maxFallingSpeed);
	else
		speed.y = MIN(speed.y, gravity);

	speed = collider->AvoidCollision(speed, *collider);
	position.x += (int)speed.x;
	position.y += (int)speed.y;

	//Collider
	collider->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	

	return true;
}

void j1Player::OnCollision(Collider* collider1, Collider* collider2) {
	/*if (collider2->type == COLLIDER_WALL) {
		if (collider1->rect.y + collider1->rect.h >= collider->rect.y) {
			LOG("%i", collider1->rect.y);
			position.y = collider2->rect.y - collider1->rect.h + 1;
			isGrounded = true;
			LOG("%i", collider1->rect.y);
		}
	}	*/
}

void j1Player::setGround(bool ground)
{
	collider->onGround = ground;
	isFalling = false;
}
