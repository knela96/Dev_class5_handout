#include "j1App.h"
#include "j1Image.h"
#include "j1ElementGUI.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Log.h"
#include "j1Slider.h"
#include "j1Fonts.h"
#include "ButtonFunctions.h"
#include "j1Textures.h"
#include "j1Animation.h"
#include "Brofiler\Brofiler.h"

j1Image::j1Image(fPoint position, SDL_Rect* anim, Levels Scene, windowType window_type, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type, bool display) :
	anim(anim),
	scene(Scene),
	animation(nullptr),
	window_type(window_type),
	display(display),
	j1ElementGUI(position, nullptr, type, graphics, parent) {

	global_pos = getParentPos(this);
	
	rect = new SDL_Rect({ (int)global_pos.x , (int)global_pos.y, anim->w , anim->h });
	
}

j1Image::j1Image(fPoint position, j1Animation* animation, windowType window_type, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type, bool display) :
	anim(nullptr),
	scene(Levels::NONE),
	animation(animation),
	window_type(window_type),
	display(display),
	j1ElementGUI(position, nullptr, type, graphics, parent) {

	global_pos = getParentPos(this);
	

	rect = new SDL_Rect({ (int)global_pos.x , (int)global_pos.y, animation->frames[0].w, animation->frames[0].h });

}


j1Image::~j1Image(){}

bool j1Image::PreUpdate() {

	BROFILER_CATEGORY("ImagePreUpdate", Profiler::Color::MediumVioletRed);

	if (to_delete)
		App->gui->deleteElement(this);
	return true;
}

bool j1Image::CleanUp() {
	//LOG("Cleaning Image");
	parent = nullptr;
	graphics = nullptr;
	delete rect;
	rect = nullptr;
	delete anim;
	anim = nullptr;
	animation = nullptr;
	return true;
}

bool j1Image::Start() {

	BROFILER_CATEGORY("ImageUpdate", Profiler::Color::MidnightBlue);

	bool ret = true;

	switch (window_type) {
	case windowType::SETTINGS:
		createSettings();
		break;
	case windowType::SETTINGS_INTRO:
		createSettingsIntro();
	}

	p2List_item<j1ElementGUI*>* item;
	for (item = childs.start; item != nullptr && ret == true; item = item->next)
	{
		ret = item->data->Start();
	}
	return ret;
}

bool j1Image::Update(float dt) {

	BROFILER_CATEGORY("ImagePostUpdate", Profiler::Color::MintCream);

	bool ret = true;

	global_pos = getParentPos(this);

	p2List_item<j1ElementGUI*>* item;
	for (item = childs.start; item != nullptr && ret == true; item = item->next)
	{
		ret = item->data->Update(dt);
	}

	if(display && animation!=nullptr)
		App->render->Blit(graphics, global_pos.x, global_pos.y, &animation->GetCurrentFrame(dt), SDL_FLIP_NONE, 1, 0.0f);
	return ret;
}

void j1Image::Draw()
{
	if(display && anim!=nullptr)
		App->render->Blit(graphics, global_pos.x,global_pos.y, anim, SDL_FLIP_NONE, 1, 0.0f);

	drawChilds();
}

void j1Image::createSettings() {
	int w, h;
	App->font->CalcSize("PAUSE", w, h);

	childs.add((j1ElementGUI*)new j1Image({ 110, -80 }, new SDL_Rect({ 391,293,262,80 }), Levels::NONE , windowType::NONE, graphics, this));
	childs.add((j1ElementGUI*)new j1Button({ 89, 38 }, "RESUME", &App->gui->button_anim, f_CloseWindow, true, graphics, this));
	childs.add((j1ElementGUI*)new j1Button({ 51, 114 }, "", &App->gui->button3_anim, f_Load, false, graphics, this));
	childs.add((j1ElementGUI*)new j1Button({ 166, 114 }, "", &App->gui->button2_anim, f_Save, true, graphics, this));
	if(scene == Levels::Scene)
		childs.add((j1ElementGUI*)new j1Button({ 89,189}, "MENU", &App->gui->button_anim, f_Scene1toMainMenu, true, graphics, this));
	else if (scene == Levels::Scene2)
		childs.add((j1ElementGUI*)new j1Button({ 89,189 }, "MENU", &App->gui->button_anim, f_Scene2toMainMenu, true, graphics, this));
	childs.add((j1ElementGUI*)new j1Slider({ 140, 65 }, MUSIC, graphics, this));
	childs.add((j1ElementGUI*)new j1Slider({ 140, 115 }, FX, graphics, this));
}

void j1Image::createSettingsIntro() {
	int w, h; 
	App->font->CalcSize("SETTINGS", w, h);
	childs.add((j1ElementGUI*)new j1Image({ 110, -80 }, new SDL_Rect({ 391, 376,262,80 }), Levels::NONE, windowType::NONE, graphics, this));
	childs.add((j1ElementGUI*)new j1Slider({ 30, 75 }, MUSIC, graphics, this));
	childs.add((j1ElementGUI*)new j1Slider({ 140, 75 }, FX, graphics, this));
	childs.add((j1ElementGUI*)new j1Button({ 180, 200 }, "BACK", &App->gui->button_anim, f_CloseWindow, true, graphics, this));
}

fPoint j1Image::GetLocalPos(int width) {
	return global_pos - getParentPos(this);
}
