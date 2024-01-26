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
#include "LoseScene.h"


#include "GuiControl.h"
#include "GuiManager.h"

Lose::Lose() : Module()
{
	name.Create("lose");
}

// Destructor
Lose::~Lose()
{}

// Called before render is available
bool Lose::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool Lose::Start()
{
	loseScreen = app->tex->Load("Assets/Textures/loose.png");

	SDL_Rect btPos = {app->scene->windowW / 2 - 120, app->scene->windowH / 2 + 120, 240,60 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT     ", btPos, this);
	exitButton->function = FunctionGUI::EXIT;
	exitButton->state = GuiControlState::DISABLED;

	return true;
}

// Called each loop iteration
bool Lose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Lose::Update(float dt)
{
	app->render->DrawTexture(loseScreen, 0, 0, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
	return true;
}

// Called each loop iteration
bool Lose::PostUpdate()
{
	bool ret = true;
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exitButton->hasToExit)
		ret = false;
	
	return ret;
}


// Called before quitting
bool Lose::CleanUp()
{
	LOG("Freeing Menu");



	return true;
}

bool Lose::OnGuiMouseClickEvent(GuiControl* control)
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
