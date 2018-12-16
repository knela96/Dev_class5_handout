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
#include "j1Player.h"
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

	////LOAD PUSHBACKS

	for (pugi::xml_node push_node = conf.child("animations").child("button").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		button_anim.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	for (pugi::xml_node push_node = conf.child("animations").child("button2").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		button2_anim.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	for (pugi::xml_node push_node = conf.child("animations").child("button3").child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		button3_anim.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}

	/*for (pugi::xml_node push_node = conf.child("animations").child(temp.GetString()).child("frame"); push_node && ret; push_node = push_node.next_sibling("frame"))
	{
		anim.PushBack({
			push_node.attribute("x").as_int(),
			push_node.attribute("y").as_int(),
			push_node.attribute("w").as_int(),
			push_node.attribute("h").as_int()
			});
	}
	anim.loop = config.child("animations").child("coin").attribute("loop").as_bool();
	anim.speed = config.child("animations").child("coin").attribute("speed").as_float();*/

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
SDL_Texture* j1Gui::GetAtlas() const
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

j1ElementGUI* j1Gui::AddLabel(fPoint pos, p2SString text, int scale)
{
	j1Label* label = new j1Label(pos, text, scale);//add position and text

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

p2SString j1Gui::convertTime(uint time) {
	p2SString s_sec, s_min, s_hour, s_total;

	uint minutes = time / 60;
	uint hours = minutes / 60;

	if (time % 60 < 10) {
		s_sec.create("0%i", time % 60);
	}
	else {
		s_sec.create("%i", time % 60);
	}
	if (minutes % 60 < 10) {
		s_min.create("0%i", minutes % 60);
	}
	else {
		s_min.create("%i", minutes % 60);
	}
	if (hours < 10) {
		s_hour.create("0%i", hours);
	}
	else {
		s_hour.create("%i", hours);
	}
	s_total += s_hour;
	s_total += ": ";
	s_total += s_min;
	s_total += ": ";
	s_total += s_sec;
	return s_total;
}

p2SString j1Gui::convertScore(uint score) {
	p2SString s_score;

	if (score < 10) {
		s_score.create("0%i", score);
	}
	else {
		s_score.create("%i", score);
	}
	return s_score;
}

void j1Gui::stateElements(j1ElementGUI* element,bool state) {
	for (int i = 0; i < elements.count(); ++i)
	{
		if (elements[i] != element)
			elements[i]->state = state;
	}
}

bool j1Gui::Save(pugi::xml_node& data) const
{
	return true;
}


bool j1Gui::Load(pugi::xml_node & data)
{
	return true;
}





// class Gui ---------------------------------------------------

