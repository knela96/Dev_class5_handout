#ifndef __j1SCENE2_H__
#define __j1SCENE2_H__

#include "j1Module.h"

struct SDL_Texture;
struct j1ElementGUI;
struct j1Label;
struct j1Image;

class j1Scene2 : public j1Module
{
public:

	j1Scene2();

	// Destructor
	virtual ~j1Scene2();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node & data);

	bool Save(pugi::xml_node & data) const;

	void CreateHUD();

	void CreateLayout();

	void UpdateLives();

private:
	p2SString map;
	iPoint cam_pos;
	Levels load_scene;
	p2SString music_path;
	p2SString fx_path;

	j1ElementGUI* settings;
	bool hud = false;
	j1Label* score;
	j1Label* timer;
	j1Image* life1;
	j1Image* life2;
	j1Image* life3;
};

#endif // __j1SCENE_H__