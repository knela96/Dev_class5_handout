#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
struct j1ElementGUI;
struct j1Label;
struct j1Image;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

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
	p2SString music_path;
	p2SString fx_path;
	iPoint cam_pos;
	Levels load_scene;

	SDL_Texture* debug_tex;
	bool hud = false;
	j1ElementGUI* settings;
	j1Label* score;
	j1Label* timer;
	j1Image* life1;
	j1Image* life2;
	j1Image* life3;
};

#endif // __j1SCENE_H__