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

	//buttons
	SDL_Rect btPosSTART = { windowW / 2 - 120, windowH / 2 , 240,60 };
	startButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START     ", btPosSTART, this);
	startButton->function = FunctionGUI::START;
	//startButton->state = GuiControlState::DISABLED;
	
	SDL_Rect btPos = { windowW / 2 - 120, windowH / 2 , 240,60 };
	exitPauseButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT     ", btPos, this);
	exitPauseButton->function = FunctionGUI::EXIT;
	exitPauseButton->state = GuiControlState::DISABLED;
	
	SDL_Rect btPosR = { windowW / 2 - 120, windowH / 2 - 240, 240,60 };
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "RESUME", btPosR, this);
	resumeButton->function = FunctionGUI::RESUME;
	resumeButton->state = GuiControlState::DISABLED;

	SDL_Rect btPosS = { windowW / 2 - 120, windowH / 2 -120, 240,60 };
	settingsPauseButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "SETTINGS", btPosS, this);
	settingsPauseButton->function = FunctionGUI::SETTINGS;
	settingsPauseButton->state = GuiControlState::DISABLED;

	//Settings
	SDL_Rect btPosM = { windowW / 2 - 120, windowH / 2 - 240, 240,60 };
	musicSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "MUSIC   ", btPosM, this);
	musicSlider->function = FunctionGUI::MUSIC;
	musicSlider->state = GuiControlState::DISABLED;
	
	SDL_Rect btPosFx = { windowW / 2 - 120, windowH / 2 -120 , 240,60 };
	fxSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "FX          ", btPosFx, this);
	fxSlider->function = FunctionGUI::FX;
	fxSlider->state = GuiControlState::DISABLED;
	
	SDL_Rect btPos3 = { windowW / 2 +40,windowH / 2 -20 , 50,50 };
	fullScreenBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Fullscreen", btPos3, this);
	fullScreenBox->function = FunctionGUI::FULLSCREEN;
	fullScreenBox->state = GuiControlState::DISABLED;
	
	SDL_Rect btPosVS = { windowW / 2 +40,windowH / 2+80  , 50,50 };
	vSyncBtn = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSYNC", btPosVS, this);
	vSyncBtn->function = FunctionGUI::VSYNC;
	vSyncBtn->state = GuiControlState::DISABLED;

	SDL_Rect btPosBP = { windowW / 2 - 120, windowH / 2+180 , 240,60 };
	backSettingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "BACK", btPosBP, this);
	backSettingsButton->function = FunctionGUI::BACKTOPAUSE;
	backSettingsButton->state = GuiControlState::DISABLED;


	//Ui Player
	SDL_Rect btPos2 = { windowW - 150, 100, 240,100 };
	playerLifesBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 1, "Lifes:", btPos2, this);
	playerLifesBox->function = FunctionGUI::LIVES;
	playerLifesBox->state = GuiControlState::DISABLED;

	//SDL_Rect btPos2 = { windowW - 150, 100, 240,100 };
	//playerLifesBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 1, "Lifes:", btPos2, this);
	//playerLifesBox->function = FunctionGUI::LIVES;
	//playerLifesBox->state = GuiControlState::DISABLED;



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

	app->render->DrawTexture(intialScreen, (app->scene->windowW / 2)*-1, (app->scene->windowH / 2)*-1 , false);

	
	// Get the mouse position and obtain the map coordinate
	app->input->GetMousePosition(mousePos.x, mousePos.y);

	//if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !player->isDead) 
	//{
	//	sameGame = true;
	//	app->SaveRequest();
	//}

	//if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !player->isDead) 
	//{
	//	app->LoadRequest();
	//}

	return true;
}

// Called each loop iteration
bool Title::PostUpdate()
{


	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exitPauseButton->hasToExit)
		ret = false;

	return ret;
}

void Title::ShowPauseButtons(bool condition)
{
	if (condition) 
	{
		exitPauseButton->state = GuiControlState::NORMAL;
		resumeButton->state = GuiControlState::NORMAL;
		settingsPauseButton->state = GuiControlState::NORMAL;
		exitPauseButton->state = GuiControlState::NORMAL;
	}
	else
	{
		exitPauseButton->state = GuiControlState::DISABLED;
		resumeButton->state = GuiControlState::DISABLED;
		settingsPauseButton->state = GuiControlState::DISABLED;
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

bool Title::LoadState(pugi::xml_node node)
{
	//configure position for player
	b2Vec2 newPos(PIXEL_TO_METERS(node.child("player").attribute("LastCheckPointX").as_int()), PIXEL_TO_METERS(node.child("player").attribute("LastCheckPointY").as_int()));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	player->lastCheckPoint.x = node.child("player").attribute("LastCheckPointX").as_int();
	player->lastCheckPoint.y = node.child("player").attribute("LastCheckPointY").as_int();
	



	int i = 0;
	for (pugi::xml_node checkPointNode = node.child("checkPoint"); checkPointNode; checkPointNode = checkPointNode.next_sibling("checkPoint"))
	{
		listOfCheckPoints[i]->isPicked = checkPointNode.attribute("isPicked").as_bool();
		i++;
	}
	i = 0;
	for (pugi::xml_node coinNode = node.child("coin"); coinNode; coinNode = coinNode.next_sibling("coin"))
	{
		bool temp = listOfCoins[i]->isCollected;
		listOfCoins[i]->isCollected = coinNode.attribute("isCollected").as_bool();
		if (!temp && temp != listOfCoins[i]->isCollected) {
			app->physics->DestroyObject(listOfCoins[i]->pbody);
		}
		i++;
	}
	i = 0;
	for (pugi::xml_node lifeItemNode = node.child("liveItem"); lifeItemNode; lifeItemNode = lifeItemNode.next_sibling("liveItem"))
	{
		bool temp = listOfItemLives[i]->isCollected;
		listOfItemLives[i]->isCollected = lifeItemNode.attribute("isCollected").as_bool();
		if (!temp && temp != listOfItemLives[i]->isCollected) {
			app->physics->DestroyObject(listOfItemLives[i]->pbody);
		}
		i++;
	}


	//Configure the position of enemies and if they are dead or not
	bool checkGame = node.child("newGame").attribute("sameGame").as_bool();
	if (sameGame != checkGame) 
	{
		newPos = b2Vec2(PIXEL_TO_METERS(node.child("flyingEnemy1").attribute("x").as_int()), PIXEL_TO_METERS(node.child("flyingEnemy1").attribute("y").as_int()));
		flyingEnemy_1->pbody->body->SetTransform(newPos, flyingEnemy_1->pbody->body->GetAngle());
		flyingEnemy_1->isOnSceen = node.child("flyingEnemy1").attribute("isOnSceen").as_bool();

		flyingEnemy_2->isOnSceen = node.child("flyingEnemy2").attribute("isOnSceen").as_bool();
		newPos = b2Vec2(PIXEL_TO_METERS(node.child("flyingEnemy2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("flyingEnemy2").attribute("y").as_int()));
		flyingEnemy_2->pbody->body->SetTransform(newPos, flyingEnemy_2->pbody->body->GetAngle());

		walkingEnemy_1->isOnSceen = node.child("enemy1").attribute("isOnSceen").as_bool();
		newPos = b2Vec2(PIXEL_TO_METERS(node.child("enemy1").attribute("x").as_int()), PIXEL_TO_METERS(node.child("enemy1").attribute("y").as_int()));
		walkingEnemy_1->pbody->body->SetTransform(newPos, walkingEnemy_1->pbody->body->GetAngle());

		walkingEnemy_2->isOnSceen = node.child("enemy2").attribute("isOnSceen").as_bool();
		newPos = b2Vec2(PIXEL_TO_METERS(node.child("enemy2").attribute("x").as_int()), PIXEL_TO_METERS(node.child("enemy2").attribute("y").as_int()));
		walkingEnemy_2->pbody->body->SetTransform(newPos, walkingEnemy_2->pbody->body->GetAngle());
		sameGame = true;
	}
	
	return true;

};

bool Title::SaveState(pugi::xml_node node)
{
	//Save if is on a current game
	pugi::xml_node gameNode = node.append_child("newGame");
	gameNode.append_attribute("sameGame").set_value(sameGame);



	//Save the checkPoints Position and if its picked
	for (int i = 0; i < listOfCheckPoints.Count(); i++)
	{
		pugi::xml_node checkPointNode = node.append_child("checkPoint");
		checkPointNode.append_attribute("isPicked").set_value(listOfCheckPoints[i]->isPicked);
		checkPointNode.append_attribute("x").set_value(listOfCheckPoints[i]->position.x);
		checkPointNode.append_attribute("y").set_value(listOfCheckPoints[i]->position.y);
	}
	
	for (int i = 0; i < listOfItemLives.Count(); i++)
	{
		pugi::xml_node checkPointNode = node.append_child("liveItem");
		checkPointNode.append_attribute("isPicked").set_value(listOfItemLives[i]->isCollected);
		checkPointNode.append_attribute("x").set_value(listOfItemLives[i]->position.x);
		checkPointNode.append_attribute("y").set_value(listOfItemLives[i]->position.y);
	}

	//Save the coins Position and if its picked
	for (int i = 0; i < listOfCoins.Count(); i++)
	{
		pugi::xml_node coinNode = node.append_child("coin");
		coinNode.append_attribute("isCollected").set_value(listOfCoins[i]->isCollected);
		coinNode.append_attribute("x").set_value(listOfCoins[i]->position.x);
		coinNode.append_attribute("y").set_value(listOfCoins[i]->position.y);
	}

	//Save player position
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("LastCheckPointX").set_value(player->lastCheckPoint.x);
	playerNode.append_attribute("LastCheckPointY").set_value(player->lastCheckPoint.y);
	//Save Enemies position
	pugi::xml_node flyingEnemy1Node = node.append_child("flyingEnemy1");
	flyingEnemy1Node.append_attribute("isOnSceen").set_value(flyingEnemy_1->isOnSceen);
	if (flyingEnemy_1->isOnSceen) {
		flyingEnemy1Node.append_attribute("x").set_value(flyingEnemy_1->position.x);
		flyingEnemy1Node.append_attribute("y").set_value(flyingEnemy_1->position.y);
	}
	else
	{
		flyingEnemy1Node.append_attribute("x").set_value(flyingEnemy_1->deathPosition.x);
		flyingEnemy1Node.append_attribute("y").set_value(flyingEnemy_1->deathPosition.y);
	}
	

	pugi::xml_node flyingEnemy2Node = node.append_child("flyingEnemy2");
	flyingEnemy2Node.append_attribute("isOnSceen").set_value(flyingEnemy_2->isOnSceen);
	if (flyingEnemy_2->isOnSceen) {
		flyingEnemy2Node.append_attribute("x").set_value(flyingEnemy_2->position.x);
		flyingEnemy2Node.append_attribute("y").set_value(flyingEnemy_2->position.y);
	}
	else
	{
		flyingEnemy2Node.append_attribute("x").set_value(flyingEnemy_2->deathPosition.x);
		flyingEnemy2Node.append_attribute("y").set_value(flyingEnemy_2->deathPosition.y);
	}

	pugi::xml_node enemy1Node = node.append_child("enemy1");
	enemy1Node.append_attribute("isOnSceen").set_value(walkingEnemy_1->isOnSceen);
	enemy1Node.append_attribute("x").set_value(walkingEnemy_1->position.x);
	enemy1Node.append_attribute("y").set_value(walkingEnemy_1->position.y);

	pugi::xml_node enemy2Node = node.append_child("enemy2");
	enemy2Node.append_attribute("isOnSceen").set_value(walkingEnemy_2->isOnSceen);
	enemy2Node.append_attribute("x").set_value(walkingEnemy_2->position.x);
	enemy2Node.append_attribute("y").set_value(walkingEnemy_2->position.y);




	return true;
};


bool Title::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->function)
	{
	case FunctionGUI::START:
		app->entityManager->active = true;
		app->map->active = true;
		app->scene->active = true;
		app->titleScreen->active = false;
		break;
	case FunctionGUI::EXIT:
		control->hasToExit = true;
		break;
	case FunctionGUI::SETTINGS:
		ShowPauseButtons(false);
		ShowSettings(true);
		break;	
	case FunctionGUI::FULLSCREEN:
		if (!isFullScreen) {
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
		break;
	case FunctionGUI::MUSIC:
		app->audio->ChangeMusicVolume(musicSlider->newValue);
		break;
	case FunctionGUI::FX:
		app->audio->ChangeFxVolume(fxSlider->newValue);
		break;
	case FunctionGUI::VSYNC:
		break;
	case FunctionGUI::TIMER:
		break;
	case FunctionGUI::RESUME:
		isOnPause = false;
		ShowPauseButtons(false);
		break;
	case FunctionGUI::BACKTOTITLE:
		break;
	case FunctionGUI::BACKTOPAUSE:
		ShowSettings(false);
		ShowPauseButtons(true);
		break;
	default:
		break;
	}
	return true;
}
