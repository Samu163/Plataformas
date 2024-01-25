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

ItemLives::ItemLives() : Entity(EntityType::LIVES_ITEM)
{
	name.Create("Lives");
}

ItemLives::~ItemLives() {}

bool ItemLives::Awake() {


	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	idleAnim.PushBack({ 0, 0, 122,91 });
	idleAnim.PushBack({ 122, 0,122,91 });
	idleAnim.PushBack({ 122+122, 0, 122,91 });
	idleAnim.PushBack({ 0 ,91, 122,91 });
	idleAnim.PushBack({ 122  ,91, 122,91 });

	idleAnim.PushBack({ 0 ,91, 122,91 });
	idleAnim.PushBack({ 122 + 122, 0, 122,91 });
	idleAnim.PushBack({ 122, 0,122,91 });

		idleAnim.speed = 0.2f;
	idleAnim.loop = true;


	pickedAnim.PushBack({ 0, 0, 122,91 });
	pickedAnim.PushBack({ 122, 0,122,91 });
	pickedAnim.PushBack({ 122 + 122, 0, 122,91 });
	pickedAnim.PushBack({ 0 ,91, 122,91 });
	pickedAnim.PushBack({ 122  ,91, 122,91 });
	pickedAnim.PushBack({ 0 ,91, 122,91 });
	pickedAnim.PushBack({ 122 + 122, 0, 122,91 });
	pickedAnim.PushBack({ 122, 0,122,91 });
	pickedAnim.speed = 1.0f;
	pickedAnim.loop = true;


	return true;
}

bool ItemLives::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 12, bodyType::STATIC);
	pbody->ctype = ColliderType::LIVES_ITEM;

	return true;
}

bool ItemLives::Update(float dt)
{
	//Checking if the player is colliding with the coin in order to destroy it 

		if (app->scene->player->currentPosition.x > position.x - 20 &&
			app->scene->player->currentPosition.x<position.x + 20 &&
			app->scene->player->currentPosition.y>position.y - 20 &&
			app->scene->player->currentPosition.y < position.y + 20)
		{
			isPickedRef = true;


		};


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

bool ItemLives::CleanUp()
{
	return true;
}