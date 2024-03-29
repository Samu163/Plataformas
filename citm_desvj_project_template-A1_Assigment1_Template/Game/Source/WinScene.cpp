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
	pantallaWin = app->tex->Load("Assets/Textures/win.png");

	SDL_Rect btPos = { app->scene->windowW / 2 - 120, app->scene->windowH / 2 + 120, 240,60 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT     ", btPos, this);
	exitButton->function = FunctionGUI::EXIT;
	exitButton->state = GuiControlState::DISABLED;

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
	app->render->DrawTexture(pantallaWin, 0, 0, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
	return true;
}

// Called each loop iteration
bool Win::PostUpdate()
{
	bool ret = true;
	if (ext == true) {
		ret = false;
	}
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exitButton->hasToExit)
		ret = false;
	
	return ret;
}

// Called before quitting
bool Win::CleanUp()
{
	LOG("Freeing Menu");
	return true;
}

bool Win::OnGuiMouseClickEvent(GuiControl* control)
{
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
