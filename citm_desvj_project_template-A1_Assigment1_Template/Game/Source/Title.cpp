#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Map.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "ItemLives.h"
#include "title.h"


#include "Defs.h"
#include "Log.h"

Title::Title() : Module()
{
	name.Create("scene");
}

// Destructor
Title::~Title()
{}

// Called before render is available
bool Title::Awake(pugi::xml_node& config)
{


	return true;
}

// Called before the first frame
bool Title::Start()
{
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");
	windowTex = app->tex->Load("Assets/Textures/bgSettings.png");
	intialScreen = app->tex->Load("Assets/Textures/Tittlebg.png");

	clickFx = app->audio->LoadFx("Assets/Audio/Fx/click.ogg");

	//buttons
	SDL_Rect btPosSTART = { windowW / 2 - 120, windowH / 2 - 240 , 240,60 };
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START     ", btPosSTART, this);
	startButton->function = FunctionGUI::START;

	SDL_Rect btPosCONT = { windowW / 2 - 120, windowH / 2 - 120 , 240,60 };
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "CONTINUE  ", btPosCONT, this);
	continueButton->function = FunctionGUI::CONTINUE;

	SDL_Rect btPosS = { windowW / 2 - 120, windowH / 2 , 240,60 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", btPosS, this);
	settingsButton->function = FunctionGUI::SETTINGS;

	SDL_Rect btPosCR = { windowW / 2 - 120, windowH / 2 + 120, 240,60 };
	creditsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "CREDITS     ", btPosCR, this);
	creditsButton->function = FunctionGUI::CREDITS;

	SDL_Rect btPos = { windowW / 2 - 120, windowH / 2 + 240, 240,60 };
	exitPauseButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT     ", btPos, this);
	exitPauseButton->function = FunctionGUI::EXIT;


	//Settings
	SDL_Rect btPosM = { windowW / 2 - 120, windowH / 2 - 240, 240,60 };
	musicSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "MUSIC   ", btPosM, this);
	musicSlider->function = FunctionGUI::MUSIC;
	musicSlider->state = GuiControlState::DISABLED;

	SDL_Rect btPosFx = { windowW / 2 - 120, windowH / 2 - 120 , 240,60 };
	fxSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "FX          ", btPosFx, this);
	fxSlider->function = FunctionGUI::FX;
	fxSlider->state = GuiControlState::DISABLED;

	SDL_Rect btPos3 = { windowW / 2 + 40,windowH / 2 - 20 , 50,50 };
	fullScreenBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Fullscreen", btPos3, this);
	fullScreenBox->function = FunctionGUI::FULLSCREEN;
	fullScreenBox->state = GuiControlState::DISABLED;

	SDL_Rect btPosVS = { windowW / 2 + 40,windowH / 2 + 120 , 50,50 };
	vSyncBtn = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSYNC", btPosVS, this);
	vSyncBtn->function = FunctionGUI::VSYNC;
	vSyncBtn->state = GuiControlState::DISABLED;

	SDL_Rect btPosBP = { windowW / 2 - 120, windowH / 2 + 240 , 240,60 };
	backSettingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "BACK", btPosBP, this);
	backSettingsButton->function = FunctionGUI::BACKTOTITLE;
	backSettingsButton->state = GuiControlState::DISABLED;




	return true;
}

// Called each loop iteration
bool Title::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Title::Update(float dt)
{
	app->scene->player->position = app->scene->player->initialPosition;




	app->render->DrawTexture(intialScreen, 0, 0, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);


	if (isOnSettings)
	{
		app->win->GetWindowSize(app->scene->windowW, app->scene->windowH);
		app->render->DrawTexture(windowTex, (app->scene->windowW / 2) - 185, (app->scene->windowH / 2) - 300, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
	}





	// Get the mouse position and obtain the map coordinate
	app->input->GetMousePosition(mousePos.x, mousePos.y);


	return true;
}

// Called each loop iteration
bool Title::PostUpdate()
{


	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exitPauseButton->hasToExit)
		ret = false;

	return ret;
}

void Title::ShowPauseButtons(bool condition)
{
	if (condition)
	{
		startButton->state = GuiControlState::NORMAL;
		continueButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;
		exitPauseButton->state = GuiControlState::NORMAL;
	}
	else
	{
		startButton->state = GuiControlState::DISABLED;
		continueButton->state = GuiControlState::DISABLED;
		settingsButton->state = GuiControlState::DISABLED;
		creditsButton->state = GuiControlState::DISABLED;
		exitPauseButton->state = GuiControlState::DISABLED;
	}
}

void Title::ShowSettings(bool condition)
{
	if (condition)
	{
		musicSlider->state = GuiControlState::NORMAL;
		fxSlider->state = GuiControlState::NORMAL;
		fullScreenBox->state = GuiControlState::NORMAL;
		backSettingsButton->state = GuiControlState::NORMAL;
		vSyncBtn->state = GuiControlState::NORMAL;
	}
	else
	{
		musicSlider->state = GuiControlState::DISABLED;
		fxSlider->state = GuiControlState::DISABLED;
		fullScreenBox->state = GuiControlState::DISABLED;
		backSettingsButton->state = GuiControlState::DISABLED;
		vSyncBtn->state = GuiControlState::DISABLED;
	}
}

// Called before quitting
bool Title::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


bool Title::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->function)
	{
	case FunctionGUI::START:

		app->audio->PlayFx(clickFx);
		//app->fadeToBlack->FadeToBlack(this, app->entityManager, 60);
		//app->fadeToBlack->FadeToBlack(this, app->map, 60);
		//app->fadeToBlack->FadeToBlack(this, app->scene, 60);

		app->entityManager->active = true;
		app->map->active = true;
		app->scene->active = true;
		app->titleScreen->active = false;
		app->scene->coinsBox->state = GuiControlState::NORMAL;
		app->scene->playerLifesBox->state = GuiControlState::NORMAL;
		ShowPauseButtons(false);
		break;
	case FunctionGUI::CONTINUE:
		app->audio->PlayFx(clickFx);
		app->entityManager->active = true;
		app->map->active = true;
		app->scene->active = true;
		app->titleScreen->active = false;
		continueBtn = true;
		ShowPauseButtons(false);
		break;
	case FunctionGUI::EXIT:
		app->audio->PlayFx(clickFx);
		control->hasToExit = true;
		break;
	case FunctionGUI::SETTINGS:
		app->audio->PlayFx(clickFx);
		isOnSettings = true;
		ShowPauseButtons(false);
		ShowSettings(true);
		break;
	case FunctionGUI::FULLSCREEN:
		app->audio->PlayFx(clickFx);
		if (!isFullScreen) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
			////TODO: Relocating all the other butons 
			//app->win->GetWindowSize(windowW, windowH);	
			//SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 240,60 };
			//exitButton->bounds = btPos;
			isFullScreen = true;
		}
		else
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
			isFullScreen = false;
		}
		break;
	case FunctionGUI::CREDITS:
		app->audio->PlayFx(clickFx);
		app->credits->counter = 0;
		app->fadeToBlack->FadeToBlack(this, app->credits, 60);
		break;
	case FunctionGUI::MUSIC:
		app->audio->PlayFx(clickFx);
		app->audio->ChangeMusicVolume(musicSlider->newValue);
		break;
	case FunctionGUI::FX:
		app->audio->PlayFx(clickFx);
		app->audio->ChangeFxVolume(fxSlider->newValue);
		break;
	case FunctionGUI::VSYNC:
		app->audio->PlayFx(clickFx);
		break;
	case FunctionGUI::TIMER:
		break;
	case FunctionGUI::RESUME:
		app->audio->PlayFx(clickFx);
		isOnPause = false;
		ShowPauseButtons(false);
		break;
	case FunctionGUI::BACKTOTITLE:
		app->audio->PlayFx(clickFx);
		isOnSettings = false;
		ShowPauseButtons(true);
		ShowSettings(false);

		break;
	case FunctionGUI::BACKTOPAUSE:
		app->audio->PlayFx(clickFx);
		ShowSettings(false);
		ShowPauseButtons(true);
		break;
	default:
		break;
	}
	return true;
}

