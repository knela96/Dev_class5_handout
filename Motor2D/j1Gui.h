#ifndef __j1GUI_H__
#define __j1GUI_H__
#include "j1Module.h"
#include "p2List.h"
#include "j1Animation.h"

#define CURSOR_WIDTH 2


// TODO 1: Create your structure of classes
class j1ElementGUI;

struct SDL_Texture;
struct SDL_Rect;

enum OrientationType;


enum class windowType {
	NONE = -1,
	SETTINGS, 
	SETTINGS_INTRO
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool deleteElement(j1ElementGUI * element);

	void clearList();

	// TODO 2: Create the factory methods
	// Gui creation functions
	SDL_Texture* GetAtlas() const;

	SDL_Texture* GetLogo() const;

	j1ElementGUI* AddImage(fPoint pos, SDL_Rect* rect, Levels Scene, windowType windowType, SDL_Texture* graphics = nullptr);

	j1ElementGUI* AddLabel(fPoint pos, p2SString text, int scale = 1);

	j1ElementGUI* AddButton(fPoint pos, p2SString text, SDL_Rect* rect, j1Animation* anim, int(*action)(void), bool active);

	j1ElementGUI* AddSlider(fPoint pos, OrientationType orientation);

	p2SString convertTime(uint time);

	p2SString convertScore(uint score);

	void stateElements(j1ElementGUI * element, bool state);

	bool Save(pugi::xml_node & data) const;

	bool Load(pugi::xml_node & data);

	//Change to Animation

public:
	p2List<j1ElementGUI*> elements;
	j1Animation button_anim;
	j1Animation button2_anim;
	j1Animation button3_anim;
	j1Animation diamond;
	bool b_settings = false;
	
private:
	bool debug = false;
	SDL_Texture* atlas;
	SDL_Texture* logo;
	p2SString atlas_file_name;
	p2SString logo_file_name;
	p2SString btn_file_name;
};

#endif // __j1GUI_H__