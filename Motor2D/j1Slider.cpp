#include "j1App.h"
#include "j1Render.h"
#include "j1Slider.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1ElementGUI.h"
#include "j1Textures.h"
#include "j1Audio.h"


j1Slider::j1Slider(fPoint position, OrientationType orientation, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type) :
	orientation(orientation),
	j1ElementGUI(position, nullptr, type, graphics, parent)
{
	slide = new j1Image({ position.x, position.y }, new SDL_Rect({ 598,0,177,32 }),Levels::NONE, windowType::NONE, graphics,this);
	thumb = new j1Image({ position.x, position.y+3 }, new SDL_Rect({ 604,34,18,26 }), Levels::NONE, windowType::NONE, graphics,this);
	label = new j1Label({ position.x - 25, position.y + 12}, "100", 1, 0, nullptr, this);
	label->text = "";

	if (OrientationType::MUSIC == orientation)
		childs.add((j1ElementGUI*)new j1Image({ position.x + 20, position.y - 65 }, new SDL_Rect({ 479,178,138,56 }), Levels::NONE, windowType::NONE, graphics, this));
	if (OrientationType::FX == orientation)
		childs.add((j1ElementGUI*)new j1Image({ position.x + 20, position.y - 65 }, new SDL_Rect({ 479,235,138,56 }), Levels::NONE, windowType::NONE, graphics, this));

	offset = 28;

	slide->rect->x += offset;
	slide->rect->w -= offset*2;

	childs.add(slide);
	childs.add(thumb);
	childs.add(label);

	if(OrientationType::MUSIC == orientation)
		setSliderValue(App->audio->v_music * 100);
	else if(OrientationType::FX == orientation)
		setSliderValue(App->audio->v_fx * 100);

	App->audio->ChangeMusicVolume();
}


j1Slider::~j1Slider()
{}

bool j1Slider::CleanUp()
{
	LOG("Cleaning Slider");
	//graphics = nullptr;
	slide = nullptr;
	thumb = nullptr;
	label = nullptr;
	parent = nullptr;
	return true;
}

bool j1Slider::Update(float dt)
{
	if (OrientationType::MUSIC == orientation)
		setSliderValue(App->audio->v_music * 100);
	else if (OrientationType::FX == orientation)
		setSliderValue(App->audio->v_fx * 100);

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	if (mouse.x >= thumb->global_pos.x && mouse.x < thumb->rect->w + thumb->global_pos.x && mouse.y >= thumb->global_pos.y && mouse.y < thumb->rect->h + thumb->global_pos.y) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			sliding = true;
		}
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		sliding = false;

	if (sliding) {
		o_mouse = mouse;

		thumb->global_pos += { o_mouse.x - (float)thumb->global_pos.x - thumb->rect->w / 2, 0 };

		if (thumb->global_pos.x <= slide->rect->x)
			thumb->global_pos.x = slide->rect->x;
		else if(thumb->global_pos.x + thumb->rect->w >= slide->rect->x + slide->rect->w)
			thumb->global_pos.x = slide->rect->x + slide->rect->w - thumb->rect->w;

	}

	thumb->rect->x = thumb->global_pos.x;
	thumb->rect->y = thumb->global_pos.y;
	
	p2SString temp = p2SString("%i", sliderValue());
	if (temp != label->text) {
		label->text = temp;
		label->UpdateText();

		float volume = (float)sliderValue() / 100;
		if (OrientationType::MUSIC == orientation)
			App->audio->v_music = volume;
		else if (OrientationType::FX == orientation)
			App->audio->v_fx = volume;
		App->audio->ChangeMusicVolume();
	}

	return true;
}

void j1Slider::Draw()
{
	drawChilds();
}

int j1Slider::sliderValue() {
	fPoint p = (thumb->global_pos - thumb->position) - thumb->getParentPos(this);
	return (int)((p.x / (float)(slide->rect->w - thumb->rect->w)) * 100) - (offset -1);
}

void j1Slider::setSliderValue(int value) {
	thumb->global_pos.x = slide->rect->x + (float)((slide->rect->w - thumb->rect->w) * ((value + 0.1) / 100));
}

