#include "ItemLives.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Player.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Lives::Lives() : Entity(EntityType::ITEMLIVES)
{
	name.Create("Lives");
}

Lives::~Lives() {}

bool Lives::Awake() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	idleAnim.PushBack({ 0, 0, 20, 20 });
	idleAnim.PushBack({ 20, 0, 20, 20 });
	idleAnim.PushBack({ 40, 0, 20, 20 });
	idleAnim.PushBack({ 60, 0, 20, 20 });
	idleAnim.PushBack({ 80, 0, 20, 20 });
	idleAnim.PushBack({ 100, 0, 20, 20 });
	idleAnim.PushBack({ 120, 0, 20, 20 });
	idleAnim.PushBack({ 140, 0, 20, 20 });
	idleAnim.speed = 0.2f;
	idleAnim.loop = true;


	pickedAnim.PushBack({ 0, 0, 20, 20 });
	pickedAnim.PushBack({ 20, 0, 20, 20 });
	pickedAnim.PushBack({ 40, 0, 20, 20 });
	pickedAnim.PushBack({ 60, 0, 20, 20 });
	pickedAnim.PushBack({ 80, 0, 20, 20 });
	pickedAnim.PushBack({ 100, 0, 20, 20 });
	pickedAnim.PushBack({ 120, 0, 20, 20 });
	pickedAnim.PushBack({ 140, 0, 20, 20 });
	pickedAnim.speed = 1.0f;
	pickedAnim.loop = true;


	return true;
}

bool Lives::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 12, bodyType::STATIC);
	pbody->ctype = ColliderType::LIVES;

	return true;
}

bool Lives::Update(float dt)
{
	//Checking if the player is colliding with the coin in order to destroy it 
	if (app->scene->player->currentPosition.x < 400) {
		if (app->scene->player->currentPosition.x > position.x - 50 &&
			app->scene->player->currentPosition.x<position.x + 50 &&
			app->scene->player->currentPosition.y>position.y - 50 &&
			app->scene->player->currentPosition.y < position.y + 50)
		{
			isPickedRef = true;


		};
	}




	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//Animations
	if (isPickedRef) {

		if (counterForAnim < 20)
		{
			currentAnimation = &pickedAnim;
			pickedAnim.Update();
			if (counterForAnim == 19)
			{
				app->physics->DestroyObject(pbody);
				isCollected = true;
			}
			counterForAnim++;
		}
	}
	else
	{
		currentAnimation = &idleAnim;
		idleAnim.Update();
	}
	//render the coin if is in the scene
	if (!isCollected)
	{
		app->render->DrawTexture(texture, position.x + 7, position.y + 7, false, &currentAnimation->GetCurrentFrame());
	}

	return true;
}

bool Lives::CleanUp()
{
	return true;
}