#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collisions.h"
#include "j1Player.h"

j1Collisions::j1Collisions() : j1Module()
{
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_POWERUP] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_POWERUP] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER_SHOT] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_POWERUP] = false;

	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_PLAYER_SHOT][COLLIDER_POWERUP] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_POWERUP] = false;

	matrix[COLLIDER_POWERUP][COLLIDER_WALL] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_POWERUP][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_PLAYER_SHOT] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_POWERUP][COLLIDER_POWERUP] = false;
}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] != nullptr && data.colliders[i]->to_delete == true)
		{
			delete data.colliders[i];
			data.colliders[i] = nullptr;
		}
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		// skip empty colliders
		if (data.colliders[i] == nullptr)
			continue;

		c1 = data.colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < data.colliders.count(); ++k)
		{
			// skip empty colliders
			if (data.colliders[k] == nullptr)
				continue;

			c2 = data.colliders[k];
			if (c2 != nullptr) {

				if (c1->CheckCollision(c2->rect) == true)
				{
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

void j1Collisions::Draw()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;


	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] == nullptr)
			continue;

		switch (data.colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(data.colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(data.colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(data.colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY: // red
			App->render->DrawQuad(data.colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLAYER_SHOT: // yellow
			App->render->DrawQuad(data.colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY_SHOT: // magenta
			App->render->DrawQuad(data.colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_POWERUP:
			App->render->DrawQuad(data.colliders[i]->rect, 255, 0, 255, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Unloading colliders");

	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		if (data.colliders[i] != nullptr)
		{
			delete data.colliders[i];
			data.colliders[i] = nullptr;
		}
	}

	return true;
}

bool j1Collisions::Update(float dt)
{
	Draw();
	return true;
}

// Load new map
bool j1Collisions::Load(pugi::xml_document& map_file)
{
	LOG("Loading Colliders");
	bool ret = true;

	// Load all colliders info ----------------------------------------------
	pugi::xml_node collider;
	for (collider = map_file.child("map").child("objectgroup"); collider && ret; collider = collider.next_sibling("objectgroup"))
	{
		for (pugi::xml_node object = collider.child("object"); object && ret; object = object.next_sibling("object"))
		{
			Collider* col = new Collider();

			col->rect.x = object.attribute("x").as_uint();
			col->rect.y = object.attribute("y").as_uint();
			col->rect.w = object.attribute("width").as_uint();
			col->rect.h = object.attribute("height").as_uint();
			col->type = (ColliderTypes)object.attribute("name").as_uint();

			data.colliders.add(col);
		}
	}

	ret = setColliders();

	return ret;
}


bool j1Collisions::setColliders() {
	for (uint i = 0; i < data.colliders.count(); ++i)
	{
		switch (data.colliders[i]->type) {
		case COLLIDER_PLAYER:
			data.colliders[i]->callback = App->player;
			App->player->collider = data.colliders[i];
			break;
		}
	}
	return true;
}

bool Collider::CheckCollision(const SDL_Rect & r) const
{
	if (r.y + r.h <= rect.y || r.y >= rect.y + rect.h || r.x + r.w <= rect.x || r.x >= rect.x + rect.w)
		return false;
	else
		return true;
}

iPoint Collider::AvoidCollision(iPoint speed, Collider& collider){

	iPoint new_speed = speed;
	Collider c1 = collider;
	c1.rect.x += speed.x;
	c1.rect.y += speed.y;
	
	for (uint i = 0; i < App->collisions->data.colliders.count(); ++i)
	{
		// skip empty colliders
		if (App->collisions->data.colliders[i] == nullptr)
			continue;

		Collider* c2 = App->collisions->data.colliders[i];

		if (c1.CheckCollision(c2->rect) == true)
		{
			if (c2->gettype() == 0) {
				new_speed = CollisionSpeed(&c1.rect, &c2->rect, new_speed);
			}
			c1.rect.y -= (speed.y - new_speed.y);
			c1.rect.x -= (speed.x - new_speed.x);
		}

	}

	collider.callback->setGround(onGround);

	return new_speed;
}

iPoint Collider::CollisionSpeed(SDL_Rect* collider1, SDL_Rect* collider2, iPoint new_speed) {
	SDL_Rect overlay;
	SDL_IntersectRect(collider1, collider2, &overlay);

	if (new_speed.y > 0) {
		if (collider1->y + collider1->h > collider2->y ) {
			if (new_speed.x > 0) {
				if (overlay.w < overlay.h && collider1->x < collider2->x + collider2->w)
					new_speed.x -= overlay.w;
				else if (overlay.w < overlay.h && collider1->x + collider1->w > collider2->x)
					new_speed.x += overlay.w;
				else if (overlay.h < overlay.w && collider1->y > collider2->y) {
					new_speed.y += overlay.h;
				}
				else if (overlay.h < overlay.w && collider1->y + collider1->h > collider2->y) {
					new_speed.y -= overlay.h;
					onGround = true;
				}
			}
			else if (new_speed.x < 0) {
				if (overlay.w < overlay.h && collider1->x < collider2->x + collider2->w)
					new_speed.x += overlay.w;
				else if (overlay.w < overlay.h && collider1->x + collider1->w > collider2->x)
					new_speed.x -= overlay.w;
				else if (overlay.h < overlay.w && collider1->y > collider2->y) {
					new_speed.y += overlay.h;
				}
				else if (overlay.h < overlay.w && collider1->y + collider1->h > collider2->y) {
					new_speed.y -= overlay.h;
					onGround = true;
				}
			}
			else {
				new_speed.y -= overlay.h;
				onGround = true;
			}
		}
		else {
			if (new_speed.x > 0) {
				if (collider1->x + collider1->w > collider2->x)
					new_speed.x -= overlay.w;
			}
			else if (new_speed.x < 0)
				if (collider1->x > collider2->x + collider2->w)
					new_speed.x += overlay.w;
		}
	}
	else if (new_speed.y < 0) {
		if (collider1->y <= collider2->y + collider2->h) {
			if (new_speed.x > 0) {
				if (overlay.h > overlay.w && collider1->x < collider2->x + collider2->w)
					new_speed.x += overlay.w;
				else if (overlay.h > overlay.w && collider1->x + collider1->w > collider2->x)
					new_speed.x -= overlay.w;
				else if (overlay.w > overlay.h && collider1->y + collider1->h > collider2->y)
					new_speed.y += overlay.h;
			}
			else if (new_speed.x < 0) {
				if (overlay.h > overlay.w && collider1->x + collider1->w > collider2->x)
					new_speed.x -= overlay.w;
				else if (overlay.h > overlay.w && collider1->x < collider2->x + collider2->w)
					new_speed.x += overlay.w;
				else if (overlay.w > overlay.h && collider1->y + collider1->h > collider2->y)
					new_speed.y += overlay.h;
			}
			else
				new_speed.y += overlay.h;
		}
	}
	else {
		if (new_speed.x > 0) {
			if (collider1->x + collider1->w > collider2->x)
				new_speed.x -= overlay.w;
		}
		else if (new_speed.x < 0)
			if (collider1->x < collider2->x + collider2->w)
				new_speed.x += overlay.w;
	}

	return new_speed;
}