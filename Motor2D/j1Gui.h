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

enum windowType;
enum OrientationType;
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

	// TODO 2: Create the factory methods
	// Gui creation functions
	const SDL_Texture* GetAtlas() const;

	j1ElementGUI* AddImage(fPoint pos, SDL_Rect* rect, windowType windowType);

	j1ElementGUI* AddLabel(fPoint pos,p2SString text);

	j1ElementGUI* AddButton(fPoint pos, p2SString text, SDL_Rect* rect, j1Animation* anim, void(*action)(void));

	j1ElementGUI* AddSlider(fPoint pos, OrientationType orientation);

	//Change to Animation

public:
	p2List<j1ElementGUI*> elements;
	j1Animation button_anim;
	
private:
	bool debug = false;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__