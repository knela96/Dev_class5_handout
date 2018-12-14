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

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	//LOAD PUSHBACKS

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	bool ret = true;
	atlas = App->tex->Load(atlas_file_name.GetString());
	
	SDL_Rect* rect = new SDL_Rect({ 33, 546,424,452 });
	fPoint pos = { 200.0f, 50.0f };
	button_anim.PushBack({ 7,118,223,57 });//sprites buttons
	
	//elements.add(AddImage(pos, rect, SETTINGS));

	p2List_item<j1ElementGUI*>* item;
	for (item = elements.start; item != nullptr && ret == true; item = item->next)
	{
		item->data->Start();
	}
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt) {

	bool ret = true;
	p2List_item<j1ElementGUI*>* item;
	for (item = elements.start; item != nullptr && ret == true; item = item->next)
	{
		item->data->Update(dt);
	}
	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;
	
	p2List_item<j1ElementGUI*>* item;
	for (item = elements.start; item != nullptr && ret == true; item = item->next)
	{
		item->data->Draw();
		if (debug) {
			item->data->DebugDraw();
		}
	}


	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

j1ElementGUI* j1Gui::AddImage(fPoint pos, SDL_Rect* rect, windowType windowType)
{
	j1Image* image = new j1Image(pos,rect, windowType, atlas);

	j1ElementGUI* element = image;
	return element;
	
}

j1ElementGUI* j1Gui::AddLabel(fPoint pos, p2SString text)
{
	j1Label* label = new j1Label(pos,text);//add position and text

	j1ElementGUI* element = (j1ElementGUI*)label;
	return element;
}

j1ElementGUI* j1Gui::AddButton(fPoint pos, p2SString text, SDL_Rect* rect, j1Animation* anim, void(*action)(void)) {
	j1Button* button = new j1Button(pos, text, anim, action, atlas);

	j1ElementGUI* element = button;
	return element;
}

j1ElementGUI* j1Gui::AddSlider(fPoint pos, OrientationType orientation) {
	j1Slider* slider = new j1Slider(pos, HORIZONTAL, atlas);

	j1ElementGUI* element = slider;
	return element;
}


// class Gui ---------------------------------------------------

