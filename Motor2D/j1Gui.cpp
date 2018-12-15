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

	//LOAD PUSHBACKS
	button_anim.PushBack({ 100,100,150,50 });

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	bool ret = true;
	atlas = App->tex->Load(atlas_file_name.GetString());
	b_settings = false;

	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != nullptr)
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
		if (elements[i] != nullptr)
			ret = elements[i]->PreUpdate();
	}
	return ret;
}

bool j1Gui::Update(float dt) {

	bool ret = true;

	for (int i = 0; i < elements.count(); ++i)
	{
		if(elements[i] != nullptr)
			ret = elements[i]->Update(dt);
	}
	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != nullptr) {
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

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		b_settings = !b_settings;
	}

	
	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	App->tex->UnLoad(atlas);
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

j1ElementGUI* j1Gui::AddImage(fPoint pos, SDL_Rect* rect, windowType windowType)
{
	j1Image* image = new j1Image(pos, rect, windowType, atlas);
	
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
	j1Slider* slider = new j1Slider(pos, HORIZONTAL, atlas);

	j1ElementGUI* element = slider;
	element->Start();
	elements.add(element);
	return element;
}



// class Gui ---------------------------------------------------

