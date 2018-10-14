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
#include "j1Map.h"

#include "SDL/include/SDL.h"

j1Player::j1Player()
{
	name.create("player");	
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
	
	speed.x = config.child("speed").attribute("x").as_uint();
	speed.y = config.child("speed").attribute("y").as_uint();

	position.x = config.child("position").attribute("x").as_uint();
	position.y = config.child("position").attribute("y").as_uint();

	life = config.child("life").attribute("value").as_uint();
	dead = config.child("dead").attribute("value").as_bool();

	jumpSpeed = config.child("jumpSpeed").attribute("value").as_float();
	maxFallingSpeed = config.child("maxFallingSpeed").attribute("value").as_float();
	walkSpeed = config.child("walkSpeed").attribute("value").as_float();
	gravity = config.child("gravity").attribute("value").as_float();

	// ANIMATIONS

	//Idle pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("idle").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_idle.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_idle.loop = config.child("animations").child("idle").attribute("loop").as_bool();
	anim_idle.speed = config.child("animations").child("idle").attribute("speed").as_float();

	//Run Pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("run").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_run.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_run.loop = config.child("animations").child("run").attribute("loop").as_bool();
	anim_run.speed = config.child("animations").child("run").attribute("speed").as_float();

	//Loading Fx
	App->audio->LoadFx(config.child("animations").child("run").child("fx").child_value());


	//Plane Pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("plane").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_plane.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_plane.loop = config.child("animations").child("plane").attribute("loop").as_bool();
	anim_plane.speed = config.child("animations").child("plane").attribute("speed").as_float();

	//Loading Fx
	App->audio->LoadFx(config.child("animations").child("plane").child("fx").child_value());

	//Death Pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("death").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_death.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_death.loop = config.child("animations").child("death").attribute("loop").as_bool();
	anim_death.speed = config.child("animations").child("death").attribute("speed").as_float();

	App->audio->LoadFx(config.child("animations").child("death").child("fx").child_value());

	//Jump Pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("jump_up").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_jumpup.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_jumpup.loop = config.child("animations").child("jump_up").attribute("loop").as_bool();
	anim_jumpup.speed = config.child("animations").child("jump_up").attribute("speed").as_float();

	App->audio->LoadFx(config.child("animations").child("jump_up").child("fx").child_value());

	for (pugi::xml_node push_node = config.child("animations").child("jump_down").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_jumpdown.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_jumpdown.loop = config.child("animations").child("jump_down").attribute("loop").as_bool();
	anim_jumpdown.speed = config.child("animations").child("jump_down").attribute("speed").as_float();


	App->audio->LoadFx(config.child("animations").child("jump_down").child("fx").child_value());

	current_animation = &anim_idle;


	return ret;
}

bool j1Player::Start() {
	
	if (!b_respawn) {
		position.x = respawn.x = collider->rect.x;
		position.y = respawn.y = collider->rect.y;
	}
	else {
		position.x = respawn.x;
		position.y = respawn.y;
		b_respawn = false;
	}

	graphics = App->tex->Load(texture_path.GetString());

	App->render->camera.x = 60 * App->win->GetScale();
	App->render->camera.y = 0;

	currentState = CharacterState::Jump;

	resetPlayer();

	return true;
}

bool j1Player::CleanUp()
{
	App->audio->StopFx();
	//App->audio->UnloadFx();
	LOG("Unloading Player assets");
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	collider = nullptr;
	return true;
}
// Update: draw background
bool j1Player::Update(float dt)
{	
	if (!dead || !win) {

		if (!death_anim) {
			if (onGround)
				lastPosition = position;
			switch (currentState)
			{
			case CharacterState::Stand:

				App->audio->StopFx();

				if (!onGround)
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
					App->audio->StopFx();
					App->audio->PlayFx(Jump_fx, 0);
					speed.y = -jumpSpeed;
					//speed.y -= 5.0f;
					currentState = CharacterState::Jump;
					onGround = false;
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
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
				{
					App->audio->StopFx();
					currentState = CharacterState::Stand;
					speed.x = 0;
					break;
				}
				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					App->audio->PlayFx(Run_fx, 1);
					flip = false;
					speed.x += 1.0f;
					if (speed.x > walkSpeed)
						speed.x = walkSpeed;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					App->audio->PlayFx(Run_fx, 1);
					flip = true;
					speed.x -= 1.0f;
					if (speed.x < -walkSpeed)
						speed.x = -walkSpeed;
				}

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				{
					App->audio->StopFx();
					App->audio->PlayFx(Jump_fx, 0); 

					speed.y = -jumpSpeed;

					//speed.y -= 1.0f;
					currentState = CharacterState::Jump;

					onGround = false;
					break;
				}
				else if (!onGround)
				{
					currentState = CharacterState::Jump;
					current_animation = &anim_jumpdown;
					App->audio->StopFx();
					break;
				}


				break;
			case CharacterState::Jump:

				current_gravity = gravity;
				if (speed.y <= 0) {
					current_animation = &anim_jumpup;
				}
				else if (speed.y > 0)
					current_animation = &anim_jumpdown;

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					if (!plane && start_time == 0) {
						start_time = SDL_GetTicks();
						plane = true;
    					App->audio->PlayFx(Plane_fx,1);
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
					if (SDL_GetTicks() - start_time < 800 && plane) {
						current_gravity = 1.0f;
						current_animation = &anim_plane;
					}
					else {
						plane = false;
						App->audio->StopFx();
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
					App->audio->StopFx();
					if (speed.y < 0.0f)
						speed.y = MAX(speed.y, -2.0f);
					if (!onGround)
						plane = false;
				}

				if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
				{
					speed.x = 0.0f;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
				{
					speed.x = 0.0f;
					break;
				}
				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					flip = false;
					speed.x = walkSpeed;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					flip = true;
					speed.x = -walkSpeed;
				}

				if (onGround)
				{
					if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
					{
						currentState = CharacterState::Stand;
						speed.x = 0;
						speed.y = 0;
						plane = false;
						start_time = 0;
					}
					else
					{
						currentState = CharacterState::Stand;
						speed.y = 0;
						plane = false;
						start_time = 0;
					}
				}
				break;
			}

			speed.y += current_gravity;
			if (!plane)
				speed.y = MIN(speed.y, maxFallingSpeed);
			else
				speed.y = MIN(speed.y, current_gravity);

			speed = collider->AvoidCollision(speed, *collider);

			position.x += (int)speed.x;
			position.y += (int)speed.y;
		}
		
		if(death_anim)
			deathAnim();

		cameraPos();

		//Animation checks
		switch (currentState) {
		case CharacterState::Walk:
			if (speed.x == 0.0f) {
				current_animation = &anim_idle;
			}
			else current_animation = &anim_run;
			break;
		case CharacterState::Stand:
			current_animation = &anim_idle;
			break;
		}
		//Collider
		if(flip)
			collider->SetPos(position.x + 4, position.y);
		else
			collider->SetPos(position.x + 10, position.y);

		// Draw everything --------------------------------------
		if(flip)
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		else
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	}
	return true;
}

bool j1Player::PostUpdate()
{
	return true;
}

void j1Player::OnCollision(Collider* collider1, Collider* collider2) {
	if (collider2->gettype() == 2) {
		if (death_anim == false) {
			if (godmode == false)
				current_life--;
			death_anim = true;
		}
	}
	else if (collider2->gettype() == 3) {
		if(!win)
			App->audio->PlayFx(Win_fx, 1);
		win = true;
	}
}

void j1Player::setGround(bool ground, bool falling)
{
	onGround = ground;
	isFalling = falling;
}

void j1Player::cameraPos()
{
	if ((position.x + collider->rect.w) * App->win->GetScale() > App->render->camera.w / 2 + App->render->camera.x + 100)
		App->render->camera.x += speed.x * App->win->GetScale();
	else if (position.x * App->win->GetScale() < App->render->camera.x + App->render->camera.w / 2 - 100 && App->render->camera.x > 60)
		App->render->camera.x += speed.x * App->win->GetScale();
	
	if (App->render->camera.y + App->render->camera.h < (App->map->data.height * 16 * App->win->GetScale()) && App->render->camera.y > 0) {
		if ((position.y + collider->rect.h) * App->win->GetScale() > App->render->camera.h + App->render->camera.y - (50 * App->win->GetScale()))
			App->render->camera.y += speed.y * App->win->GetScale();
		else if (position.y * App->win->GetScale() < App->render->camera.y + (50 * App->win->GetScale()))
			App->render->camera.y += speed.y * App->win->GetScale();
		else
			App->render->camera.y = (position.y * App->win->GetScale() + collider->rect.h / 2 - App->win->height / 2);
	}
}

void j1Player::deathAnim()
{
	current_animation = &anim_death;

	App->audio->PlayFx(Death_fx, 1);

	if (position.y > App->render->camera.y + App->render->camera.h - App->render->camera.h / 4 && !isFalling && death_anim) {
		position.y -= 20;
		start_time = SDL_GetTicks();
	}
	else if (SDL_GetTicks() - start_time > 500)
		isFalling = true;

	if (isFalling && death_anim) {
		position.y += 20;
	}

	if (isFalling && position.y > App->render->camera.y + App->render->camera.h) {
		isFalling = false;
		death_anim = false;

		if (current_life <= 0)
			dead = true;
		else {
			position = lastPosition;
			App->render->camera.x = position.x * App->win->GetScale() - 300;
		}
	}

}

void j1Player::resetPlayer()
{
	flip = false;
	dead = false;
	win = false;
	death_anim = false;
	plane = false;
	current_life = life;
}

// Load Game State
bool j1Player::Load(pugi::xml_node& data)
{
	speed.x = data.child("speed").attribute("x").as_uint();
	speed.y = data.child("speed").attribute("y").as_uint();

	position.x = data.child("position").attribute("x").as_uint();
	position.y = data.child("position").attribute("y").as_uint();

	life = data.child("life").attribute("value").as_uint();

	jumpSpeed = data.child("jumpSpeed").attribute("value").as_float();
	maxFallingSpeed = data.child("maxFallingSpeed").attribute("value").as_float();
	walkSpeed = data.child("walkSpeed").attribute("value").as_float();
	gravity = data.child("gravity").attribute("value").as_float();

	return true;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data;

	player.append_child("speed").append_attribute("x") = speed.x;
	player.child("speed").append_attribute("y") = speed.y;

	player.append_child("position").append_attribute("x") = position.x;
	player.child("position").append_attribute("y") = position.y;

	player.append_child("life").append_attribute("value") = life;

	player.append_child("jumpSpeed").append_attribute("value") = jumpSpeed;
	player.append_child("maxFallingSpeed").append_attribute("value") = maxFallingSpeed;
	player.append_child("walkSpeed").append_attribute("value") = walkSpeed;
	player.append_child("gravity").append_attribute("value") = gravity;


	return true;
}