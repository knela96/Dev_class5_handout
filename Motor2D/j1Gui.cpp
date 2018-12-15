#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Image.h"
#include "j1Label.h"
#include "j1Button.h"
#include "j1Slider.h"
#include "j1Audio.h"
#include "ButtonFunctions.h"



j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string(); 
	logo_file_name = conf.child("logo").attribute("file").as_string();

	//LOAD PUSHBACKS
	button_anim.PushBack({ 483,0,114,58 });
	button_anim.PushBack({ 483,60,114,58 });
	button_anim.PushBack({ 483,121,114,58 });

	button2_anim.PushBack({ 699,60,76,58 });
	button2_anim.PushBack({ 699,120,76,58 });
	button2_anim.PushBack({ 699,181,76,58 });

	button3_anim.PushBack({ 619,60,76,58 });
	button3_anim.PushBack({ 619,120,76,58 });
	button3_anim.PushBack({ 619,181,76,58 });

	btn_file_name = conf.child("btn_fx").child_value();


	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	bool ret = true;
	atlas = App->tex->Load(atlas_file_name.GetString());
	logo = App->tex->Load(logo_file_name.GetString());

	App->audio->LoadFx(btn_file_name.GetString());


	b_settings = false;

	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != nullptr && elements[i]->state)
			elements[i]->Start();
	}
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;
	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != nullptr && elements[i]->state)
			ret = elements[i]->PreUpdate();
	}
	return ret;
}

bool j1Gui::Update(float dt) {

	bool ret = true;

	for (int i = 0; i < elements.count() && ret != false; ++i)
	{
		if(elements[i] != nullptr && elements[i]->state)
			ret = elements[i]->Update(dt);
	}
	for (int i = 0; i < elements.count() && ret != false; ++i)
	{
		if (elements[i] != nullptr && elements[i]->state) {
			elements[i]->Draw();
			if (debug) {
				elements[i]->DebugDraw();
			}
		}
	}
	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;
	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	App->tex->UnLoad(atlas);
	App->audio->UnloadFx();
	atlas = nullptr;

	for (uint i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != nullptr) {
			elements[i]->CleanChilds();
			delete elements[i];
			elements[i] = nullptr;
		}
	}
	elements.clear();
	return true;
}

bool j1Gui::deleteElement(j1ElementGUI* element)
{
	if (element != nullptr)
	{
		for (uint i = 0; i < elements.count(); ++i)
		{
			if (elements[i] == element)
			{
				elements[i]->CleanChilds();
				delete elements[i];
				elements[i] = nullptr;
				clearList();
			}
		}
	}
	return false;
}

void j1Gui::clearList() {
	p2List_item<j1ElementGUI*>* item = elements.start;
	for (uint i = 0; i < elements.count(); ++i)
	{
		if (elements[i] == nullptr)
			elements.del(item);
		item = item->next;
	}
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

SDL_Texture * j1Gui::GetLogo() const
{
	return logo;
}

j1ElementGUI* j1Gui::AddImage(fPoint pos, SDL_Rect* rect, Levels scene, windowType windowType, SDL_Texture* graphics)
{
	j1Image* image;
	if(graphics == nullptr)
		image = new j1Image(pos, rect, scene, windowType, atlas);
	else
		image = new j1Image(pos, rect, scene, windowType, graphics);
	
	j1ElementGUI* element = image;
	element->Start();
	elements.add(element);
	return element;
	
}

j1ElementGUI* j1Gui::AddLabel(fPoint pos, p2SString text)
{
	j1Label* label = new j1Label(pos, text);//add position and text

	j1ElementGUI* element = (j1ElementGUI*)label;
	element->Start();
	elements.add(element);
	return element;
}

j1ElementGUI* j1Gui::AddButton(fPoint pos, p2SString text, SDL_Rect* rect, j1Animation* anim, int(*action)(void), bool active) {
	j1Button* button = new j1Button(pos, text, anim, action, active, atlas);

	j1ElementGUI* element = button;
	element->Start();
	elements.add(element);
	return element;
}

j1ElementGUI* j1Gui::AddSlider(fPoint pos, OrientationType orientation) {
	j1Slider* slider = new j1Slider(pos, MUSIC, atlas);

	j1ElementGUI* element = slider;
	element->Start();
	elements.add(element);
	return element;
}

void j1Gui::stateElements(j1ElementGUI* element,bool state) {
	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != element)
			elements[i]->state = state;
	}
}

bool j1Gui::Load(pugi::xml_node & data)
{

	return true;
}




// class Gui ---------------------------------------------------

