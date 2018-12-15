#pragma once

#include "j1ElementGUI.h"
#include "j1Animation.h"

class j1Image : public j1ElementGUI
{
public:
	j1Image(fPoint position, SDL_Rect* anim, Levels Scene, windowType windowType = windowType::NONE, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::IMAGE);
	~j1Image();

	bool PreUpdate();

	bool CleanUp();
	
	bool Start();

	bool Update(float dt);

	void Draw();

	fPoint GetLocalPos(int i);

	void enableLoad();

	void createSettings();

	void createSettingsIntro();

public:
	windowType window_type;
	SDL_Rect* anim;
	Levels scene;
};