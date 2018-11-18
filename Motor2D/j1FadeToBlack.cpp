#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "j1FadeToBlack.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "Brofiler/Brofiler.h"

j1FadeToBlack::j1FadeToBlack()
{
	name.create("fade");	
}

j1FadeToBlack::~j1FadeToBlack()
{}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen"); 
	screen = { 0, 0, (int)(App->win->width * App->win->scale), (int)(App->win->height * App->win->scale) };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	resetStep();
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	BROFILER_CATEGORY("FadeToBlackUpdate", Profiler::Color::DeepPink);

	if(current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch(current_step)
	{
		case fade_step::fade_to_black:
		{
			if(now >= total_time)
			{
				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::fade_from_black;
				if (switch_scene) {
					module_off->Disable();
					module_on->Enable();
				}
				else {
					isBlack = true;
				}
			}
		} break;

		case fade_step::fade_only_black:
		{
			if (now >= total_time)
			{
				isBlack = true;
			}
		} break;

		case fade_step::fade_from_black:
		{
			isBlack = false;
			normalized = 1.0f - normalized;
			if (now >= total_time) {
				current_step = fade_step::none;
				switch_scene = false;
				isfadding = false;
			}

		} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;
	
	if(current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		
		ret = true;
		this->module_off = module_off;
		this->module_on = module_on;
		switch_scene = true;
	}
	else if (isfadding){

		/*	If the background is fading to the second texture of the background 
		*	it will continue the fade to black and load the level, if it's from the 
		*	black it will return to black and load the level
		*/
		if (current_step == fade_step::fade_from_black) {
			current_step = fade_step::fade_to_black;
			start_time = (SDL_GetTicks() - start_time) + (SDL_GetTicks() - total_time);
		}
		isfadding = false;
		ret = true;
		this->module_off = module_off;
		this->module_on = module_on;
		switch_scene = true;
	}

	return ret;
}

bool j1FadeToBlack::FadeToBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
		isfadding = true;
	}

	return ret;
}

bool j1FadeToBlack::FadeonlyBlack(float time)
{
	bool ret = false;

	if (current_step == fade_step::none && !isBlack)
	{
		current_step = fade_step::fade_only_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 1000.0f);
		ret = true;
		isfadding = true;
	}

	return ret;
}

void j1FadeToBlack::resetStep()
{
	current_step = fade_step::none;
}
