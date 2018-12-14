#include "j1App.h"
#include "j1Image.h"
#include "j1ElementGUI.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Log.h"
#include "j1Slider.h"
#include "ButtonFunctions.h"

j1Image::j1Image(fPoint position, SDL_Rect* anim, windowType window_type, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type) :
	anim(anim),
	window_type(window_type),
	j1ElementGUI(position, nullptr, type, graphics, parent) {

	global_pos = getParentPos(this);
	
	rect = new SDL_Rect({ (int)global_pos.x , (int)global_pos.y, anim->w, anim->h });
	
}


j1Image::~j1Image(){}

bool j1Image::Start() {
	bool ret = true;

	switch (window_type) {
	case SETTINGS:
		createSettings();
		break;
	}

	p2List_item<j1ElementGUI*>* item;
	for (item = childs.start; item != nullptr && ret == true; item = item->next)
	{
		ret = item->data->Start();
	}
	return ret;
}

bool j1Image::Update(float dt) {
	bool ret = true;

	global_pos = getParentPos(this);

	p2List_item<j1ElementGUI*>* item;
	for (item = childs.start; item != nullptr && ret == true; item = item->next)
	{
		ret = item->data->Update(dt);
	}
	return ret;
}

void j1Image::Draw()
{
	rect->x = global_pos.x;
	rect->y = global_pos.y;
	App->render->Blit(graphics, global_pos.x,global_pos.y, anim, SDL_FLIP_NONE, 1, 0.0f);

	drawChilds();
}

void j1Image::createSettings() {
	childs.add((j1ElementGUI*)new j1Button({ 0,100 }, "hola", &App->gui->button_anim, action, graphics, this));
	childs.add((j1ElementGUI*)new j1Slider({ 0, 150 }, HORIZONTAL, graphics, this));
}

fPoint j1Image::GetLocalPos(int width) {
	return global_pos - getParentPos(this);
}