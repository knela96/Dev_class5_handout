#pragma once

#include "j1ElementGUI.h"
#include "j1Animation.h"

class j1Image : public j1ElementGUI
{
public:
	j1Image(fPoint position, SDL_Rect* anim, Levels Scene, windowType windowType = windowType::NONE, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::IMAGE,bool display = true);
	j1Image(fPoint position, j1Animation * anim, windowType windowType = windowType::NONE, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::IMAGE, bool display = true);
	~j1Image();

	bool PreUpdate();

	bool CleanUp();
	
	bool Start();

	bool Update(float dt);

	void Draw();

	fPoint GetLocalPos(int i);
	
	void createSettings();

	void createSettingsIntro();

public:
	windowType window_type;
	SDL_Rect* anim;
	j1Animation* animation;
	Levels scene;
	bool display;
};