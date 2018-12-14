#pragma once

#include "j1ElementGUI.h"
#include "j1Button.h"
#include "p2Point.h"
#include "j1Image.h"

enum OrientationType {
	HORIZONTAL,
	VERTICAL
};
class j1Slider : public j1ElementGUI
{
public:
	j1Slider(fPoint position, OrientationType orientation, SDL_Texture* graphics = nullptr, j1ElementGUI* parent = nullptr, ElementUIType type = ElementUIType::SLIDER);
	~j1Slider();

	bool Update(float dt);

	void Draw();

	int sliderValue();

	void setSliderValue(int value);

public:
	OrientationType orientation;
	iPoint o_mouse;
	iPoint slide_pos; //0 to 1  pos/height * 100
	j1Image* slide;
	j1Image* thumb;
	uint slide_value;
	j1Label* label;
	//j1Button* thumb;
	//SDL_Rect* text_rect;
	//p2SString text;
private:
	bool sliding = false;
};