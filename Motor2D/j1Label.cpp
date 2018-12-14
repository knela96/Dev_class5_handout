#include "j1App.h"
#include "j1Render.h"
#include "j1Label.h"
#include "j1Fonts.h"
#include "p2Log.h"


j1Label::j1Label(fPoint position, p2SString text, SDL_Texture* graphics, j1ElementGUI* parent, ElementUIType type) :
	text(text),
	j1ElementGUI(position,nullptr, type,graphics,parent)
{
	App->font->CalcSize(text.GetString(), width, height);

	global_pos = getParentPos(this);
	
	rect = new SDL_Rect({ (int)global_pos.x, (int)global_pos.y,width,height });
}


j1Label::~j1Label()
{}

void j1Label::Draw()
{
	this->graphics = App->font->Print(text.GetString());

	global_pos = getParentPos(this);

	rect->x = global_pos.x;
	rect->y = global_pos.y;
	App->render->Blit(graphics, global_pos.x, global_pos.y, nullptr, SDL_FLIP_NONE, 1, 0.0f);
}

/*void j1Label::DebugDraw()
{
	App->render->DrawQuad(*rect, 255, 0, 0, alpha, false, false);
}*/

