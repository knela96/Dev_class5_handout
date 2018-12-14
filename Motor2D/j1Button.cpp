#include "j1App.h"
#include "j1Button.h"
#include "j1ElementGUI.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"



j1Button::j1Button(fPoint position, p2SString text, j1Animation* anim, void(*action)(void), SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type) :
	anim(anim),
	action(action),
	j1ElementGUI(position, nullptr, type, graphics, parent)
{
	label = new j1Label({ 0 ,0 }, text, nullptr, this);
}

bool j1Button::Start() {

	global_pos = getParentPos(this);
	
	rect = new SDL_Rect({ (int)global_pos.x, (int)global_pos.y,anim->GetFrameRect(0).w,anim->GetFrameRect(0).h });

	label->position = { (float)rect->w / 2 - label->width / 2,(float)rect->h / 2 - label->height / 2 };

	childs.add((j1ElementGUI*)label);

	current_animation = &anim->GetFrameRect(0);

	return true;
}

j1Button::~j1Button()
{
	delete rect;
	rect = nullptr;
	delete anim;
	rect = nullptr;
}

bool j1Button::Update(float dt) {
	global_pos = getParentPos(this);
	
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y); 
	current_animation = &anim->GetFrameRect(0);

	if (mouse.x >= global_pos.x && mouse.x < rect->w + global_pos.x && mouse.y >= global_pos.y && mouse.y < rect->h + global_pos.y) {
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			onClick();
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			onAction();
		else
			onHover();
	}

	return true;
}

void j1Button::Draw()
{
		rect->x = global_pos.x;
		rect->y = global_pos.y;
		App->render->Blit(graphics, global_pos.x, global_pos.y , current_animation, SDL_FLIP_NONE, 1, 0.0f);
	

	p2List_item<j1ElementGUI*>* item;
	for (item = childs.start; item != nullptr; item = item->next)
	{
		item->data->Draw();
	}
}

void j1Button::onHover()
{
	LOG("HOVER");//CHANGE TEXTURE TO PRESSED TEXTURE
	current_animation = &anim->GetFrameRect(1);
}

void j1Button::onClick()
{
	LOG("CLICK");//CHANGE TEXTURE TO PRESSED TEXTURE
	current_animation = &anim->GetFrameRect(2);
}

void j1Button::onAction()
{
	LOG("ACTION");//CHANGE TEXTURE TO PRESSED TEXTURE
	action();
}