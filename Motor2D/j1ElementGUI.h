#ifndef __j1ElementGUI_H__
#define __j1ElementGUI_H__

#include "p2Point.h"
#include "j1Gui.h"
#include "j1Render.h"

enum class ElementUIType {
	NONE = -1,
	BUTTON,
	CHECKBOX,
	INPUT_TEXT,
	LABEL,
	IMAGE,
	SLIDER
};

// ---------------------------------------------------
class j1ElementGUI
{
public:

	j1ElementGUI(fPoint position, SDL_Rect* rect, ElementUIType type, SDL_Texture* graphics, j1ElementGUI* parent) : position(position), scale(scale), rect(rect), type(type), graphics(graphics), parent(parent){}

	// Destructor
	virtual ~j1ElementGUI() {}

	// Called when before render is available
	virtual bool Awake(pugi::xml_node&) { return true; }

	virtual bool Start() { return true; }

	virtual bool PreUpdate() { return true; }

	virtual bool Update() { return true; }

	virtual bool Update(float dt) { return true; }

	virtual bool PostUpdate() { return true; }

	virtual bool CleanUp() { return true; }

	virtual void Draw() {}

	//virtual void DebugDraw() {}

	virtual void setPosition() {}

	fPoint getParentPos(j1ElementGUI* element) {
		j1ElementGUI* c_parent = element->parent;
		fPoint global_pos = { 0,0 };
		while (c_parent != nullptr) {
			global_pos += c_parent->position;
			c_parent = c_parent->parent;
		}
		global_pos += element->position;
		return global_pos;
	}

	void drawChilds() {
		p2List_item<j1ElementGUI*>* item = childs.start;
		while (item != nullptr) {
			item->data->Draw();
			item = item->next;
		}
	}

	void CleanChilds() {
		for (uint i = 0; i < childs.count(); ++i)
		{
			if (childs[i] != nullptr)
			{
				childs[i]->CleanChilds();
				delete childs[i];
				childs[i] = nullptr;
			}
		}
		childs.clear();

		CleanUp();
	}

	void DebugDraw()
	{
		if(rect != nullptr)
			App->render->DrawQuad(*rect, 255, 0, 0, alpha, false, false);

		p2List_item<j1ElementGUI*>* item = childs.start;
		while (item != nullptr) {
			item->data->DebugDraw();
			item = item->next;
		}
	}



public:
	p2SString name;//delete?
	p2SString folder;//delete?
	fPoint position;
	ElementUIType type;
	SDL_Texture* graphics;
	j1ElementGUI* parent;
	SDL_Rect* rect;
	p2List<j1ElementGUI*> childs;
	uint8_t alpha = 80;
	fPoint global_pos;
	int scale = 1;

private:
};

#endif

