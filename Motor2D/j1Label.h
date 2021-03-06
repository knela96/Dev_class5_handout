#pragma once

#include "j1ElementGUI.h"

class j1Label : public j1ElementGUI
{
public:
	j1Label(fPoint position, p2SString text, int scale = 1, int move = 0, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::LABEL);
	~j1Label();

	bool CleanUp();

	
	void Draw();

	void UpdateText();

	//void DebugDraw();

public:
	p2SString text;
	int height;
	int width;
	int scale;
	int move;
};

