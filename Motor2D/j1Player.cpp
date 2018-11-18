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
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Brofiler\Brofiler.h"

#include "SDL/include/SDL.h"

j1Player::j1Player(SDL_Rect* collider_rect) : j1Entity(collider_rect)
{
	name.create("player");

	collider = App->collisions->AddCollider(*collider_rect, ColliderTypes::COLLIDER_PLAYER, (j1Module*)App->entitymanager);
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

	// attack pushbacks
	for (pugi::xml_node push_node = config.child("animations").child("attack").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim_attack.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim_attack.loop = config.child("animations").child("attack").attribute("loop").as_bool();
	anim_attack.speed = config.child("animations").child("attack").attribute("speed").as_float();


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

	App->render->camera.x = -60 * App->win->GetScale();
	App->render->camera.y = 0;



	currentState = CharacterState::Jump;

	resetPlayer();

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading Player assets");
	App->audio->StopFx();
	App->audio->UnloadFx();
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	App->collisions->deleteCollider(collider);
	collider = nullptr;
	return true;
}

// Update: draw background
bool j1Player::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("PlayerUpdate1", Profiler::Color::MediumSlateBlue);

	if (dt == 0) {
		position.x = respawn.x;
		position.y = respawn.y;
	}
	OnGround = App->collisions->CheckGroundCollision(collider);
	if (OnGround)
		isFalling = false;

	if (!dead || !win) {
		if (!death_anim && !godmode) {
			if (OnGround)
				lastPosition = position;
			
			switch (currentState)
			{
			case CharacterState::Stand:

				if (!OnGround)
				{
					currentState = CharacterState::Jump;
				}

				//if left or right key is pressed, but not both
				if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
				{
					currentState = CharacterState::Walk;
				}
				else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				{
					App->audio->StopFx();
					App->audio->PlayFx(Jump_fx, 0);
					current_animation = &anim_jumpup;
					speed.y -= 8000.0f *dt;
					currentState = CharacterState::Jump;
					OnGround = false;
				}
				else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && !attacked) {
					currentState = CharacterState::Attack;
				}

				break;
			case CharacterState::Walk:

				if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && !attacked) {
					speed = { 0,0 };

					currentState = CharacterState::Attack;
					break;
				}
				if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
				{
					currentState = CharacterState::Stand;
					speed = { 0,0 };
				}
				else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				{
					App->audio->PlayFx(Run_fx, 1);
					flip = false;
					speed.x = walkSpeed;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				{
					App->audio->PlayFx(Run_fx, 1);
					flip = true;
					speed.x = -walkSpeed;
				}
				
				
				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
				{
					current_animation = &anim_jumpup;
					App->audio->StopFx();
					App->audio->PlayFx(Jump_fx, 0);
					speed.y -= 8000.0f *dt;
					currentState = CharacterState::Jump;
					OnGround = false;
				}
				else if (!OnGround)
				{
					currentState = CharacterState::Jump;
					current_animation = &anim_jumpdown;
				}
				break;
			case CharacterState::Jump:

				speed.y += gravity*dt;

				if (speed.y <= 0) {
					current_animation = &anim_jumpup;
				}
				else if (speed.y > 0) {
					if(!plane)
						App->audio->StopFx();
					current_animation = &anim_jumpdown;
				}

				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					if (!plane && start_time == 0) {
						start_time = SDL_GetTicks();
						plane = true;
						isFalling = true;
						App->audio->StopFx();
						App->audio->PlayFx(Plane_fx, 1);
					}
				}
				else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
					if (speed.y > -jumpSpeed && !isFalling) {
						speed.y -= 4000.0f *dt;
					}
					else if(!isFalling){
 						isFalling = true;
					}

					if (SDL_GetTicks() - start_time < 800 && plane) {
						speed.y = gravity * 0.1 * dt;
						current_animation = &anim_plane;
					}
					else if(plane) {
						plane = false;
						App->audio->StopFx();
					}
				}else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
					if (plane) {
						plane = false; 
						App->audio->StopFx();
					}
					isFalling = true;
				}


				if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN && !attacked) {
					Fall = true;
					speed = { 0,0 };
					currentState = CharacterState::Attack;
				}
				else if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
				{
					speed.x = 0.0f;
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


				if (OnGround)
				{
					attacked = false;
					if (App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))
					{
						currentState = CharacterState::Stand;
						speed = { 0,0 };
						plane = false;
						start_time = 0;
					}
					else
					{
						currentState = CharacterState::Walk;
 						speed = { 0,0 };
						plane = false;
						start_time = 0;
					}
				}
				break;
			case CharacterState::Attack:
				LOG("Attack % i", anim_attack.getFrame());
				if (anim_attack.getFrame() >= 8)//CHANGE FIX
				{
					anim_attack.Reset();
					LOG("Attack");
					if (!OnGround)
					{
						attacked = true;
						currentState = CharacterState::Jump;
					}

					if (App->input->GetKey(SDL_SCANCODE_D) != App->input->GetKey(SDL_SCANCODE_A))
					{
						currentState = CharacterState::Walk;
					}
					else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
					{
						App->audio->StopFx();
						App->audio->PlayFx(Jump_fx, 0);
						current_animation = &anim_jumpup;
						speed.y -= 8000.0f *dt;
						currentState = CharacterState::Jump;
						OnGround = false;
					}
					else {
						currentState = CharacterState::Stand;
					}
					//Destroy Collider		
					App->collisions->deleteCollider(attack_col);
					attack_col = nullptr;
				}
				else {
					current_animation = &anim_attack;//DELETE
					if (attack_col == nullptr){
						if (flip)
							attack_col = App->collisions->AddCollider({ (int)position.x - 16,(int)position.y, 20, 31 }, ColliderTypes::COLLIDER_PLAYER_SHOT, (j1Module*)App->entitymanager);
						else
							attack_col = App->collisions->AddCollider({ (int)position.x + 16,(int)position.y, 20, 31 }, ColliderTypes::COLLIDER_PLAYER_SHOT, (j1Module*)App->entitymanager);
					}
				}
				break;
			}
			
			if (speed.y > maxFallingSpeed)
				speed.y = maxFallingSpeed;


			if (position.x < 0)
				position.x = 0;
			else
				position.x += speed.x * dt;

			if (position.y < 0)
				position.y = 0;
			else
				position.y += speed.y * dt;
		}
		
		if(death_anim)
			deathAnim(dt);

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
		case CharacterState::Attack:
			current_animation = &anim_attack;
			break;
		}

		if (godmode) {
			if ((App->input->GetKey(SDL_SCANCODE_A) == App->input->GetKey(SDL_SCANCODE_D))) {
				speed.x = 0;
			}
			else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				flip = true;
				speed.x = -walkSpeed;
			}
			else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				flip = false;
				speed.x = walkSpeed;
			}
			if ((App->input->GetKey(SDL_SCANCODE_W) == App->input->GetKey(SDL_SCANCODE_S))) {
				speed.y = 0;
			}
			else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				speed.y = -walkSpeed;
			}
			else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				speed.y = walkSpeed;
			}
			if (position.x < 0)
				position.x = 0;
			else
				position.x += speed.x * dt;

			if (position.y <= 0)
				position.y = 0;
			else
				position.y += speed.y * dt;

			current_animation = &anim_plane;
		}

		//Collider
		if(flip)
			collider->SetPos(position.x, position.y);//+4
		else
			collider->SetPos(position.x, position.y);//+10

		animation_Rect = current_animation->GetCurrentFrame(dt);

	}
	return true;
}

bool j1Player::Update() {

	BROFILER_CATEGORY("PlayerUpdate2", Profiler::Color::MediumSpringGreen);

	// Draw everything --------------------------------------
	if (hit)
		hitanim();

	if (!blink){
		if (flip)
			if (current_animation == &anim_attack)
				App->render->Blit(graphics, position.x - 30, position.y, &animation_Rect, SDL_FLIP_HORIZONTAL);
			else
				App->render->Blit(graphics, position.x, position.y, &animation_Rect, SDL_FLIP_HORIZONTAL);
		else if (current_animation == &anim_plane || current_animation == &anim_jumpup || current_animation == &anim_jumpdown)
			App->render->Blit(graphics, position.x - 10, position.y, &animation_Rect, SDL_FLIP_NONE);
		else
			App->render->Blit(graphics, position.x - 20, position.y, &animation_Rect, SDL_FLIP_NONE);
	}
	return true;
}

void j1Player::OnCollision(Collider* collider1, Collider* collider2) {

	if(collider2->gettype() == 0) {
		WallCollision(collider1, collider2);
	}
	else if (collider2->gettype() == COLLIDER_ENEMY) {
		if (death_anim == false && collider2->gettype() == COLLIDER_ENEMY) {
			if (godmode == false)
				current_life--;
			death_anim = true;
			App->audio->StopFx();
			App->audio->PlayFx(Death_fx, 1);
		}
		else {
			current_life--;
		}
	}
	else if (collider2->gettype() == COLLIDER_FLYING_ENEMY && !godmode || collider2->gettype() == COLLIDER_PLATFORM_ENEMY && !death_anim && !godmode) {

		if (!hit && c_blink == 0 && death_anim == false) {
			LOG("HIT"); 
			start_time = SDL_GetTicks() - start_time;
			hit = true;
			current_life--;
			if (current_life == 0) {
				hit = false;
				currentState = CharacterState::Jump;
				death_anim = true;
				App->audio->StopFx();
				App->audio->PlayFx(Death_fx, 1);
			}
			else {
				App->audio->StopFx();
				App->audio->PlayFx(Jump_fx, 0);
			}
		}
	}
	else if (collider2->gettype() == 3) {
		if (!win) 
			App->audio->PlayFx(Win_fx, 1);
		win = true;
		
	}
}

void j1Player::WallCollision(Collider* c1, Collider* c2)
{
	SDL_Rect collisionOverlay;
	SDL_IntersectRect(&c1->rect, &c2->rect, &collisionOverlay);

	if (collisionOverlay.w >= collisionOverlay.h) {
		if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y < c2->rect.y ) {	//Ground
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y--;
			}
			current_animation = &anim_idle;
			speed.y = 0.0f;
			OnGround = true; 
			isFalling = false;
		}
		else if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + c1->rect.h > c2->rect.y + c2->rect.h ) {	//Ceiling
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.y++;
			}
			speed.y = 0.0f;
			isFalling = true;
		}
		position.y = c1->rect.y;
	}
	else {
		if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x < c2->rect.x ) {	//Right
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x--;
			}
			
			speed.x = 0.0f;
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + c1->rect.w > c2->rect.x + c2->rect.w ) {	//Left
			while (c1->CheckCollision(c2->rect) == true) {
				c1->rect.x++;
			}			
			speed.x = 0.0f;
		}
		position.x = c1->rect.x;
	}
}

void j1Player::setGround(bool ground, bool falling)
{
	OnGround = ground;
	if(OnGround)
		isFalling = true;
	else
		isFalling = false;
}

void j1Player::cameraPos()
{
	if (-(position.x + collider->rect.w) <= App->render->camera.x + -App->render->camera.w + 200 && App->render->camera.x < 0)
		App->render->camera.x = App->render->camera.w - 200 + -(position.x + collider->rect.w);
	else if (-(position.x) >= App->render->camera.x  - 200)
		App->render->camera.x = 200 + -position.x;

	/*if (App->render->camera.y + App->render->camera.h < (App->map->data.height * 16 * App->win->GetScale()) && -App->render->camera.y > 0) {
		if ((position.y + collider->rect.h) * App->win->GetScale() > App->render->camera.h + App->render->camera.y - (50 * App->win->GetScale()))
			App->render->camera.y -= speed.y * App->win->GetScale();
		else if (position.y * App->win->GetScale() < App->render->camera.y + (50 * App->win->GetScale()))
			App->render->camera.y -= speed.y * App->win->GetScale();
		else
			App->render->camera.y = (position.y * App->win->GetScale() + collider->rect.h / 2 - App->win->height / 2);
	}*/
}
void j1Player::hitanim()
{
	if (SDL_GetTicks() - start_time > 100 && c_blink < 10) {
		start_time = SDL_GetTicks();
		if(c_blink % 2 == 0)
			blink = true,
			LOG("1");
		else
			blink = false,
			LOG("0");
		c_blink++;
	}
	if(c_blink == 10) {
		hit = false;
		c_blink = 0;
	}
}


void j1Player::deathAnim(float dt)
{
	current_animation = &anim_death;
	
	if (position.y > App->render->camera.y + App->render->camera.h - App->render->camera.h / 4 && !isFalling && death_anim) {
		position.y -= 800 * dt;
		start_time = SDL_GetTicks();
	}
	else if (SDL_GetTicks() - start_time > 500)
		isFalling = true;

	if (isFalling && death_anim) {
		position.y += 800 * dt;
	}

	if (isFalling && position.y > App->render->camera.y + App->render->camera.h) {
		isFalling = false;
		death_anim = false;

		if (current_life <= 0)
			dead = true;
		else {
			position = lastPosition;
			App->render->camera.x = -position.x * App->win->GetScale() + 300;
			if (App->render->camera.x > 0)
				App->render->camera.x = 0;
			start_time = SDL_GetTicks();
		}
	}

}

void j1Player::resetPlayer()
{
	flip = false;
	dead = false;
	win = false;
	death_anim = false;
	isFalling = true;
	OnGround = false;
	plane = false;
	hit = false;
	start_time = 0;
	aux_time = 0;
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
