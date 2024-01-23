#include "CheckPoint.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

CheckPoint::CheckPoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkPoint");

}

CheckPoint::~CheckPoint() {}

bool CheckPoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	upAnim.PushBack({ 0, 0, 46, 79 });

	downAnim.PushBack({ 46, 0, 46, 79 });

	risingAnim.PushBack({ 46, 0, 46, 79 });
	risingAnim.PushBack({ 46*2, 0, 46, 79 });
	risingAnim.PushBack({ 46*3, 0, 46, 79 });
	risingAnim.PushBack({ 0, 0, 46, 79 });
	risingAnim.speed = 0.2f;
	risingAnim.loop = false;


	return true;
}

bool CheckPoint::Start() {
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 40, position.y + 40, 40, bodyType::STATIC);
	pbody->ctype = ColliderType::CHECKPOINT;

	return true;
}

bool CheckPoint::Update(float dt)
{
	//standar anims
	if (isPicked) {
		currentAnimation = &upAnim;
	}
	else
	{
		currentAnimation = &downAnim;
	}
	//Check position of the last checkpoint player and activate flag if so 
	if (app->scene->player->lastCheckPoint.x > METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 40 &&
		app->scene->player->lastCheckPoint.x<METERS_TO_PIXELS(pbody->body->GetTransform().p.x) + 40 &&
		app->scene->player->lastCheckPoint.y>METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 40 &&
		app->scene->player->lastCheckPoint.y < METERS_TO_PIXELS(pbody->body->GetTransform().p.y) + 40
		&& !isPicked)
	{
		counter++;
		currentAnimation = &risingAnim;
		//Ending the animation 
		if (counter > 20)
		{
			currentAnimation = &upAnim;
			risingAnim.Reset();
			//for not entering more here 
			isPicked = true;
		}
		risingAnim.Update();
	}

	//updating position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//rendering
	app->render->DrawTexture(texture, position.x, position.y-20, false, &currentAnimation->GetCurrentFrame());

	return true;
}

bool CheckPoint::CleanUp()
{
	return true;
}

