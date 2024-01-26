#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Defs.h"
#include "Log.h"
#include "WinScene.h"


#include "GuiControl.h"
#include "GuiManager.h"

Win::Win() : Module()
{
	name.Create("win");
}

// Destructor
Win::~Win()
{}

// Called before render is available
bool Win::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;


	

	return ret;
}

// Called before the first frame
bool Win::Start()
{
	pantallaWin = app->tex->Load("Assets/Win.png");


	return true;
}

// Called each loop iteration
bool Win::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Win::Update(float dt)
{
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->render->DrawTexture(pantallaWin, 0, 0, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
	app->titleScreen->exitPauseButton->state == GuiControlState::NORMAL;
	return true;
}

// Called each loop iteration
bool Win::PostUpdate()
{
	bool ret = true;
	if (ext == true) {
		ret = false;
	}
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	
	return ret;
}


// Called before quitting
bool Win::CleanUp()
{
	LOG("Freeing Menu");
	if (app->titleScreen->exitPauseButton->hasToExit) {
		return false;
	}
	return true;
}

bool Win::OnGuiMouseClickEvent(GuiControl* control)
{
	uint height = 0;
	uint width = 0;

	switch (control->function)
	{

	case FunctionGUI::EXIT:
		app->audio->PlayFx(app->scene->clickFx);

		control->hasToExit = true;
		break;

	default:
		break;
	}
	return true;
}
