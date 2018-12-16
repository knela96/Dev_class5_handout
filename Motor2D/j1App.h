#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Scene2;
class j1SceneIntro;
class j1Map;
class j1EntityManager;
class j1Player;
class j1FadeToBlack;
class j1Collisions;
class j1PathFinding;
class j1Fonts;
class j1Gui;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;
	void j1App::calcTime(uint& timer, j1PerfTimer& ptimer);

	pugi::xml_node GetSaveData();

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

private:


	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:
	pugi::xml_document save_gamedata;
	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1PathFinding*		path;
	j1Scene*			scene;
	j1Scene2*			scene2;
	j1SceneIntro*			sceneintro;
	j1Map*				map;
	j1Collisions*		collisions;
	j1FadeToBlack*		fade;
	j1EntityManager*	entitymanager;
	j1Gui*				gui;
	j1Fonts*			font;
	float previous_time;
	float current_time;
	uint				frame_cap;
	float accumulated = 0;
	float				dt;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	uint				started_at;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				avg_fps;
	float				seconds_since_startup;
	uint32				last_frame_ms;
	uint32				frames_on_last_update;
	bool				cap_frames = true;
	int					capped_ms;

};

extern j1App* App;

#endif