#include "j1App.h"
#include "j1Button.h"
#include "j1ElementGUI.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1EntityManager.h"



j1Button::j1Button(fPoint position, p2SString text, j1Animation* anim, int(*action)(void),bool active, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type) :
	anim(anim),
	activate(active),
	action(action),
	j1ElementGUI(position, nullptr, type, graphics, parent)
{
	label = new j1Label({ 0 ,0 }, text, nullptr, this);
}

bool j1Button::Start() {

	global_pos = getParentPos(this);
	
	rect = new SDL_Rect({ (int)global_pos.x, (int)global_pos.y,anim->GetFrameRect(0).w,anim->GetFrameRect(0).h  });

	label->position = { (float)rect->w / 2 - label->width / 2,(float)rect->h / 2 - label->height / 2 };

	childs.add((j1ElementGUI*)label);

	current_animation = &anim->GetFrameRect(0);

	if (!activate) {
		if (App->GetSaveData().child("entities").child("player") == nullptr)
			active = false;
		else
			active = true;
	}else
		active = true;

	return true;
}

j1Button::~j1Button()
{}

bool j1Button::CleanUp() {
	//LOG("Cleaning Button");
	graphics = nullptr;
	delete rect;
	rect = nullptr;
	anim = nullptr;
	parent = nullptr;
	current_animation = nullptr;
	return true;
}

bool j1Button::Update(float dt) {
	bool ret = true;
	global_pos = getParentPos(this);
	
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y); 
	current_animation = &anim->GetFrameRect(0);
	
	if (!active) {
		current_animation = &anim->GetFrameRect(2);
	}
	else {
		if (mouse.x >= global_pos.x && mouse.x < rect->w + global_pos.x && mouse.y >= global_pos.y && mouse.y < rect->h + global_pos.y) {
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
				onClick();
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				ret = onAction();
			else
				onHover();
		}
	}


	return ret;
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
	current_animation = &anim->GetFrameRect(1);
}

void j1Button::onClick()
{
	current_animation = &anim->GetFrameRect(2);
}

bool j1Button::onAction()
{
	int ret = action();
	if (ret == -1) {
		parent->to_delete = true;
		return true;
	}
	return (bool)ret;
}