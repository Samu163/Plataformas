#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Pathfinding.h"



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
	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/
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
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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
	float camSpeed = 1;
	//Debug
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		isInDebugMode=!isInDebugMode;
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
		
		if (player->position.y <= 0) 
		{

		}
		else if (player->position.y >= app->win->screenSurface->h) 
		{
			player->position.y -= app->win->screenSurface->h - 13 * 2;
		}
		else
		{
			player->position.y = 0;
		}
		app->render->camera.y = -player->position.y;
		
	}
	// Get the mouse position and obtain the map coordinate
	app->input->GetMousePosition(mousePos.x, mousePos.y);


	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && !player->isDead) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !player->isDead) app->LoadRequest();

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

	

	// Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'

	// L13: Get the latest calculated path and draw


	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::LoadState(pugi::xml_node node) 
{
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();
	b2Vec2 newPos(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y));
	player->pbody->body->SetTransform(newPos, player->pbody->body->GetAngle());
	return true;

};

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);


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
