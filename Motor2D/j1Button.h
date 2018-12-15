#pragma once

#include "j1ElementGUI.h"
#include "j1Label.h"
#include "j1Animation.h"


struct SDL_Rect;

class j1Button : public j1ElementGUI
{
public:
	j1Button(fPoint pos, p2SString text, j1Animation* anim, int(*action)(void), bool active, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::BUTTON);
	~j1Button();

	bool CleanUp();

	bool Start();

	bool Update(float dt);

	void Draw();

	//void DebugDraw();

	void onHover();
	void onClick();
	bool onAction();

public:
	bool clicked = false;
	j1Label* label;
	j1Animation* anim;
	SDL_Rect* current_animation;
	int(*action)(void);
	bool activate;
	bool active;
};

