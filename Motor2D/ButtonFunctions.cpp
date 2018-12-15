#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FadeToBlack.h"
#include "j1SceneIntro.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1ElementGUI.h"
#include "ButtonFunctions.h"
#include "j1EntityManager.h"

int f_Start() {
	App->fade->FadeToBlack(App->sceneintro, App->scene);
	return true;
}


int f_Continue() {
	App->LoadGame();
	return true;
}

int f_Settings() {
	App->gui->AddImage({ 0,0 }, new SDL_Rect({ 120,0,196,196 }), Levels::NONE, windowType::SETTINGS);
	return true;
}

int f_Exit() {
	return false;
}

int f_Credits() {
	ShellExecuteA(NULL, "open", "http://google.es", NULL, NULL, SW_SHOWNORMAL);
	return true;
}

int f_CloseWindow() {
	App->gui->b_settings = false;
	return -1;
}

int f_Scene1toMainMenu() {
	App->fade->FadeToBlack(App->scene, App->sceneintro);
	return true;
}

int f_Scene2toMainMenu() {
	App->fade->FadeToBlack(App->scene2, App->sceneintro);
	return true;
}

int f_Save() {
	App->SaveGame();
	return true;
}

int f_Load() {
	App->LoadGame();
	return true;
}