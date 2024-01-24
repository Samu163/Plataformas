#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiManager.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	


	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
		listOfCoins.Add(item);
	}
	for (pugi::xml_node checkPointNode = config.child("checkPoint"); checkPointNode; checkPointNode = checkPointNode.next_sibling("checkPoint"))
	{
		CheckPoint* checkPoint = (CheckPoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkPoint->parameters = checkPointNode;
		listOfCheckPoints.Add(checkPoint);

	}


	app->map->path = config.child("map").attribute("path").as_string();
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	if (config.child("flyingEnemy1")) {
		flyingEnemy_1 = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		flyingEnemy_1->parameters = config.child("flyingEnemy1");
	}
	if (config.child("flyingEnemy2")) {
		flyingEnemy_2 = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		flyingEnemy_2->parameters = config.child("flyingEnemy2");
	}
	if (config.child("enemy1")) {
		walkingEnemy_1 = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKING_ENEMY);
		walkingEnemy_1->parameters = config.child("enemy1");
	}
	if (config.child("enemy2")) {
		walkingEnemy_2 = (Enemy*)app->entityManager->CreateEntity(EntityType::WALKING_ENEMY);
		walkingEnemy_2->parameters = config.child("enemy2");
	}
	
	return ret;
}





// Called before the first frame
bool Scene::Start()
{


	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Musica.wav", 0.0f);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->map->Load();



	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");



	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 240,60 };
	exitPauseButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "EXIT", btPos, this);
	exitPauseButton->function = FunctionGUI::FULLSCREEN;
	exitPauseButton->state = GuiControlState::DISABLED;
	musicSlider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "MUSIC", btPos, this);
	musicSlider->function = FunctionGUI::MUSIC;
	musicSlider->state = GuiControlState::DISABLED;
	SDL_Rect btPos2 = { windowW - 150, 100, 240,100 };
	playerLifesBox = (GuiControlValueBox*)app->guiManager->CreateGuiControl(GuiControlType::VALUEBOX, 1, "Lifes:", btPos2, this);
	playerLifesBox->function = FunctionGUI::LIVES;
	SDL_Rect btPos3 = { windowW - 150, 400, 50,50 };
	fullScreenBox = (GuiControlCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "Fullscreen", btPos3, this);
	fullScreenBox->function = FunctionGUI::FULLSCREEN;
	//fullScreenBox->state = GuiControlState::DISABLED;


	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	string strPlayerLifes = std::to_string(player->lifes);
	playerLifesBox->SetValue(strPlayerLifes);	
	string coins = std::to_string(player->coinCount);
	playerLifesBox->SetValue(coins);

	float camSpeed = 1;
	
	//Debug
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		isInDebugMode=!isInDebugMode;
	}
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		isOnPause = !isOnPause;
		ShowPauseButtons(isOnPause);
	}
	

	if (isInDebugMode)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);


	}
	else
	{
		if (player->position.x < app->win->screenSurface->w / 2) {
			player->position.x = app->win->screenSurface->w / 2;
		}
		app->render->camera.x = -player->position.x + app->win->screenSurface->w / 2;
		
		 if (player->position.y >= app->win->screenSurface->h/1.1) 
		{
			player->position.y -= app->win->screenSurface->h - 13 * 29;
		}
		else
		{
			player->position.y = 10;
		}

		app->render->camera.y = -player->position.y;
		
	}
	
	// Get the mouse position and obtain the map coordinate
	app->input->GetMousePosition(mousePos.x, mousePos.y);


	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !player->isDead) 
	{
		sameGame = true;
		app->SaveRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !player->isDead) 
	{
		app->LoadRequest();
	}

	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY,);

	return true;
}
void Scene::DrawPath() {
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		if (path != NULL)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			if (app->physics->debugMode) {
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y, false);

			}
		}

	}
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exitPauseButton->hasToExit)
		ret = false;

	return ret;
}

void Scene::ShowPauseButtons(bool condition)
{
	if (condition) 
	{
		exitPauseButton->state = GuiControlState::NORMAL;
	}
	else
	{
		exitPauseButton->state = GuiControlState::DISABLED;
	}
}

void Scene::ShowSettings(bool condition)
{
	if (condition) 
	{
		exitPauseButton->state = GuiControlState::NORMAL;
	}
	else
	{
		exitPauseButton->state = GuiControlState::DISABLED;
	}
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node node) 
{
	//configure position for player
	b2Vec2 newPos(PIXEL_TO_METERS(node.child("player").attribute("LastCheckPointX").as_int()), PIXEL_TO_METERS(node.child("player").attribute("LastCheckPointY").as_int()));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	player->lastCheckPoint.x = newPos.x;
	player->lastCheckPoint.y = newPos.y;
	



	int i = 0;
	for (pugi::xml_node checkPointNode = node.child("checkPoint"); checkPointNode; checkPointNode = checkPointNode.next_sibling("checkPoint"))
	{
		listOfCheckPoints[i]->isPicked = checkPointNode.attribute("isPicked").as_bool();
		i++;
	}
	i = 0;
	for (pugi::xml_node coinNode = node.child("coin"); coinNode; coinNode = coinNode.next_sibling("coin"))
	{
		listOfCoins[i]->isCollected = coinNode.attribute("isCollected").as_bool();
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

bool Scene::SaveState(pugi::xml_node node)
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


b2Vec2 Scene::CheckTheMovementWithPath(iPoint positionOfThePath, iPoint originalPosition)
{
	if (positionOfThePath.x > originalPosition.x)
	{
		if (positionOfThePath.y < originalPosition.y) {
			return b2Vec2(3, -3);
		}
		else if (positionOfThePath.y > originalPosition.y)
		{
			return b2Vec2(3, 3);
		}
		else
		{
			return b2Vec2(3, -0.165);
		}
	}
	else if (positionOfThePath.x < originalPosition.x)
	{
		if (positionOfThePath.y < originalPosition.y) {
			return b2Vec2(-3, -3);
		}
		else if (positionOfThePath.y > originalPosition.y)
		{
			return b2Vec2(-3, 3);
		}
		else
		{
			return b2Vec2(-3, -0.165);
		}
	}
	else
	{
		if (positionOfThePath.y < originalPosition.y) {
			return b2Vec2(0, -3);
		}
		else if (positionOfThePath.y > originalPosition.y)
		{
			return b2Vec2(0, 3);
		}
		else
		{
			return b2Vec2(0, -0.165);
		}
	}
}

bool Scene::CheckVelocityForFlip(b2Vec2 vel) {

	if (vel.x < 0) {
		return true;
	}
	else
	{
		return false;
	}

}


bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->function)
	{
	case FunctionGUI::START:
		break;
	case FunctionGUI::EXIT:
		control->hasToExit = true;
		break;
	case FunctionGUI::SETTINGS:
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
		break;
	case FunctionGUI::FX:
		break;
	case FunctionGUI::VSYNC:
		break;
	case FunctionGUI::TIMER:
		break;
	case FunctionGUI::RESUME:
		break;
	case FunctionGUI::BACKTOTITLE:
		break;
	default:
		break;
	}
	return true;
}
