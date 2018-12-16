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
#include "j1Player.h"

int f_Start() {
	if (App->entitymanager->GetPlayer() != nullptr)
		App->getTime(App->entitymanager->GetPlayer()->timer);
	App->fade->FadeToBlack(App->sceneintro, App->scene);
	return true;
}


int f_Continue() {
	App->LoadGame();
	return true;
}

int f_Settings() {
	App->gui->stateElements(App->gui->AddImage({ (float)(App->render->camera.w / 2) - 241, (float)(App->render->camera.h / 2) - 146 }, new SDL_Rect({ 0,0,482,293 }), nullptr, Levels::NONE, windowType::SETTINGS_INTRO),false);
	return true;
}

int f_Exit() {
	return false;
}

int f_Credits() {
	ShellExecuteA(NULL, "open", "https://knela96.github.io/Tails-Odyssey/", NULL, NULL, SW_SHOWNORMAL);
	return true;
}

int f_CloseWindow() {
	App->gui->b_settings = false;
	App->gui->stateElements(nullptr, true);
	return -1;
}

int f_Scene1toMainMenu() {

	App->gui->b_settings = true;
	App->fade->FadeToBlack(App->scene, App->sceneintro);
	return true;
}

int f_Scene2toMainMenu() {

	App->gui->b_settings = true;
	App->fade->FadeToBlack(App->scene2, App->sceneintro);
	return true;
}

int f_Save() {
	App->SaveGame();
	return 3;
}

int f_Load() {
	App->LoadGame();
	return true;
}