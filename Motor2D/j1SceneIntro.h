#ifndef __j1SCENEINTRO_H__
#define __j1SCENEINTRO_H__

#include "j1Module.h"
#include "j1Animation.h"

struct SDL_Texture;
struct j1ElementGUI;

class j1SceneIntro : public j1Module
{
public:

	j1SceneIntro();

	// Destructor
	virtual ~j1SceneIntro();

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

	void CreateHUD();

private:
	p2SString map;
	p2SString music_path;
	p2SString fx_path;
	iPoint cam_pos;
	iPoint mousepos_y;
	Levels load_scene;

	SDL_Texture* debug_tex;


	int cameraoffset;
	SDL_Texture * graphics;
	j1Animation * current_anim;
	SDL_Rect animation_Rect;
	j1Animation anim_plane;

	j1ElementGUI* settings;
	bool main_menu = false;
};

#endif // __j1SCENE_H__