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

	camera_offset.x = App->render->camera.x;
	camera_offset.y = App->render->camera.y;

	speed.x = config.child("speed").attribute("x").as_uint();
	speed.y = config.child("speed").attribute("y").as_uint();

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
		idle.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}

	idle.loop = config.child("animations").child("idle").attribute("loop").as_bool();
	idle.speed = config.child("animations").child("idle").attribute("speed").as_float();

	//Run Pushbacks
	//Plane Pushbacks
	//Detah Pushbacks

	current_animation = &idle;


	return ret;
}

bool j1Player::Start() {
	
	position.x = collider->rect.x;
	position.y = collider->rect.y;

	graphics = App->tex->Load(texture_path.GetString());

	App->render->camera.y = 0;
	App->render->camera.x = 60;

	currentState = CharacterState::Jump;

	flip = false;

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading Player assets");
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	collider = nullptr;
	return true;
}
// Update: draw background
bool j1Player::Update(float dt)
{	
	if (!dead) {
		if (onGround)
			lastPosition = position;

		switch (currentState)
		{
		case CharacterState::Stand:

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
			}else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			{
				currentState = CharacterState::Stand;
				speed.x = 0;
				break;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				flip = false;
				speed.x +=  1.0f;
				if (speed.x > walkSpeed)
					speed.x = walkSpeed;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				flip = true;
				speed.x -= 1.0f;
				if (speed.x < -walkSpeed)
					speed.x = -walkSpeed;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				speed.y = -jumpSpeed;

				//speed.y -= 1.0f;
				currentState = CharacterState::Jump;
				onGround = false;
				break;
			}
			else if (!onGround)
			{
				currentState = CharacterState::Jump;
				break;
			}
			break;
		case CharacterState::Jump:

			current_gravity = gravity;

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				plane = true;

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				if (plane)
					current_gravity = 1.0f;
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
				}
				else 
				{
					currentState = CharacterState::Stand;
					speed.y = 0;
					plane = false;
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

			if ((position.x + collider->rect.w) * App->win->GetScale() > App->render->camera.w / 2 + App->render->camera.x + 100)
				App->render->camera.x += speed.x * App->win->GetScale();
			else if (position.x * App->win->GetScale() < App->render->camera.x + App->render->camera.w / 2 - 100 && App->render->camera.x > 60)
				App->render->camera.x += speed.x * App->win->GetScale();
		if (App->render->camera.y + App->render->camera.h < (App->map->data.height *16 * App->win->GetScale()) && App->render->camera.y > 0) {
			if ((position.y + collider->rect.h) * App->win->GetScale() > App->render->camera.h + App->render->camera.y - (50 * App->win->GetScale()))
				App->render->camera.y += speed.y * App->win->GetScale();
			else if (position.y * App->win->GetScale() < App->render->camera.y + (50 * App->win->GetScale()))
				App->render->camera.y += speed.y * App->win->GetScale();
			else
				App->render->camera.y = (position.y * App->win->GetScale() + collider->rect.h / 2 - App->win->height / 2);
		}


		//Collider
		if(flip)
			collider->SetPos(position.x + 7, position.y);
		else
			collider->SetPos(position.x + 25, position.y);


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
		if (godmode == false)
			life--,	position = lastPosition, App->render->camera.x = position.x * App->win->GetScale() - 300;
		else
			position = lastPosition;
	}else if (collider2->gettype() == 3) {
		LOG("CHANGE MAP");
	}
	if (life <= 0)
		dead = true;
}

void j1Player::setGround(bool ground, bool falling)
{
	onGround = ground;
	isFalling = falling;
}
