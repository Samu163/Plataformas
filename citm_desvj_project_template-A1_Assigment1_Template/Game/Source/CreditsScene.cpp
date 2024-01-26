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
#include "CreditsScene.h"


#include "Defs.h"
#include "Log.h"

Credits::Credits() : Module()
{
	name.Create("scene");
}

// Destructor
Credits::~Credits()
{}

// Called before render is available
bool Credits::Awake(pugi::xml_node& config)
{
	return true;
}

// Called before the first frame
bool Credits::Start()
{
	app->titleScreen->active = false;
	app->winscene->active = false;
	app->loseScene->active = false;
	app->entityManager->active = false;
	app->map->active = false;
	app->scene->active = false;

	windowTex = app->tex->Load("Assets/Textures/Credits.png");

	return true;
}

// Called each loop iteration
bool Credits::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Credits::Update(float dt)
{
	app->render->DrawTexture(windowTex, 0, 0, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);

	if (counter > 20) {
		app->fadeToBlack->FadeToBlack(this, app->titleScreen, 60);

	}
	else
	{
		counter++;
	}



	return true;
}

