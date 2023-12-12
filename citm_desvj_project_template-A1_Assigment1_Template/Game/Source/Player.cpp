#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include <list>



Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//Size of each cell of the spritesheet: 122x 91y
	//Size of each cell of the spritesheet (Enemy): 72x 59y
	idleAnim.PushBack({ 0, 0, 122, 91 });
	idleAnim.PushBack({ 122, 0, 122, 91 });
	idleAnim.PushBack({ 244, 0, 122, 91 });
	idleAnim.PushBack({ 366, 0, 122, 91 });
	idleAnim.PushBack({ 488, 0, 122, 91 });
	idleAnim.PushBack({ 610, 0, 122, 91 });
	idleAnim.PushBack({ 732, 0, 122, 91 });
	idleAnim.PushBack({ 854, 0, 122, 91 });
	idleAnim.PushBack({ 976, 0, 122, 91 });
	idleAnim.speed = 0.2f;
	idleAnim.loop = true;

	walkAnim.PushBack({ 1098, 0, 122, 91 });
	walkAnim.PushBack({ 1220, 0, 122, 91 });
	walkAnim.PushBack({ 1342, 0, 122, 91 });
	walkAnim.PushBack({ 1464, 0, 122, 91 });
	walkAnim.PushBack({ 1586, 0, 122, 91 });
	walkAnim.speed = 0.2f;
	walkAnim.loop = true;

	jumpAnim.PushBack({ 122, 91, 122, 91 });
	jumpAnim.PushBack({ 244, 91, 122, 91 });
	jumpAnim.PushBack({ 366, 91, 122, 91 });
	jumpAnim.PushBack({ 488, 91, 122, 91 });
	jumpAnim.PushBack({ 610, 91, 122, 91 });
	jumpAnim.speed = 0.15f;
	jumpAnim.loop = false;

	iceBallAnim.PushBack({ 122, 91, 122, 91 });
	iceBallAnim.PushBack({ 244, 91, 122, 91 });
	iceBallAnim.PushBack({ 366, 91, 122, 91 });
	iceBallAnim.PushBack({ 488, 91, 122, 91 });
	iceBallAnim.PushBack({ 610, 91, 122, 91 });
	iceBallAnim.speed = 0.15f;
	iceBallAnim.loop = true;

	deathIceBallAnim.PushBack({ 0, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 122, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 244, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 366, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 488, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 610, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 732, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 854, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 976, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 1098, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 1220, 182, 122, 91 });
	deathIceBallAnim.PushBack({ 1342, 182, 122, 91 });
	deathIceBallAnim.speed = 0.2f;
	deathIceBallAnim.loop = false;

	deathAnim.PushBack({ 0, 182, 122, 91 });
	deathAnim.PushBack({ 122, 182, 122, 91 });
	deathAnim.PushBack({ 244, 182, 122, 91 });
	deathAnim.PushBack({ 366, 182, 122, 91 });
	deathAnim.PushBack({ 488, 182, 122, 91 });
	deathAnim.PushBack({ 610, 182, 122, 91 });
	deathAnim.PushBack({ 732, 182, 122, 91 });
	deathAnim.PushBack({ 854, 182, 122, 91 });
	deathAnim.PushBack({ 976, 182, 122, 91 });
	deathAnim.PushBack({ 1098, 182, 122, 91 });
	deathAnim.PushBack({ 1220, 182, 122, 91 });
	deathAnim.PushBack({ 1342, 182, 122, 91 });
	deathAnim.speed = 0.2f;
	deathAnim.loop = false;

	return true;
}

bool Player::Start()
{
	//initilize textures
	texture = app->tex->Load("Assets/Textures/playerIce.png");
	iceBallTexture = app->tex->Load("Assets/Textures/enemy.png");
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	//initialize player parameters
	Init();
	return true;
}

void Player::Init() 
{
	//Init function that initialize most of the player parameters
	speed = 0.4f;
	jumpingCounter = 0;
	counterForIceBalls = playerCooldown;
	isJumping = false;
	isDead = false;
	initialPosition = position;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
}

bool Player::Update(float dt)
{
	if (iceBallToDestroyIndex != -1) 
	{
		app->render->DrawTexture(texture, METERS_TO_PIXELS(listOfIceBalls[iceBallToDestroyIndex]->body->GetTransform().p.x), METERS_TO_PIXELS(listOfIceBalls[iceBallToDestroyIndex]->body->GetTransform().p.y), isFlipped, &iceBallAnimations[iceBallToDestroyIndex]->GetCurrentFrame(), zoomFactor);
		app->physics->DestroyObject(listOfIceBalls[iceBallToDestroyIndex]);
		listOfIceBalls.Del(listOfIceBalls.At(iceBallToDestroyIndex));
		iceBallToDestroyIndex = -1;
	}

	//Debug
	//Restart from initial position
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		if (isDead) 
		{
			deathAnim.Reset();
		}
		app->physics->DestroyObject(pbody);
		position = initialPosition;
		Init();
	}
	//GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	if (!isDead) 
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			isJumping = true;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && counterForIceBalls >= playerCooldown)
		{
			PhysBody* iceBall = new PhysBody();
			b2Vec2 iceBallVel = b2Vec2(20, 0);
			currentIceBallAnimation = &iceBallAnim;
			currentIceBallAnimation->Reset();
			iceBallAnimations.Add(currentIceBallAnimation);
		    if (isFlipped) 
			{
				iceBallVel = b2Vec2(-20, 0);
				iceBall = app->physics->CreateCircle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 20, METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10, 15, bodyType::DYNAMIC);
			}
			else
			{
				iceBall = app->physics->CreateCircle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) + 20, METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10, 15, bodyType::DYNAMIC);
			}
			iceBall->body->SetLinearVelocity(iceBallVel);
			iceBall->listener = this;
			iceBall->ctype = ColliderType::PROYECTILE;
			listOfIceBalls.Add(iceBall);
			counterForIceBalls = 0;

			
			
		}
		
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		{
			isFlipped = true;
			currentAnimation = &walkAnim;
			walkAnim.Update();

			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFlipped = false;
			currentAnimation = &walkAnim;
			walkAnim.Update();

			vel = b2Vec2(speed * dt, -GRAVITY_Y);
		}
		else
		{
			currentAnimation = &idleAnim;
			idleAnim.Update();
		}
		//Jump Function with dt (is not working with 30fps)
		if (isJumping) 
		{

			currentAnimation = &jumpAnim;
			jumpAnim.Update();

			int posYExtra = GRAVITY_Y / 2 + jumpingCounter - dt;
			if (posYExtra >= -GRAVITY_Y) {
				posYExtra = -GRAVITY_Y;
			}

			vel = b2Vec2(0, posYExtra);
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				vel = b2Vec2(-speed * dt, posYExtra);

			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				vel = b2Vec2(speed * dt, posYExtra);

			}
			jumpingCounter++;

		}
		else
		{
			jumpAnim.Reset();
			jumpingCounter = 0;
		}
	}
	else
	{
		zoomFactor = 1.0f;
		currentAnimation = &deathAnim;
		deathAnim.Update();
	}
	

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//Draw texture
	app->render->DrawTexture(texture, position.x-45, position.y-40, isFlipped ,&currentAnimation->GetCurrentFrame(), zoomFactor);

	if (listOfIceBalls.Count() > 0) 
	{
		for (int i = 0; i < listOfIceBalls.Count(); i++)
		{
			if (currentIceBallAnimation != NULL && listOfIceBalls[i]!=NULL)
			{
				app->render->DrawTexture(texture, METERS_TO_PIXELS(listOfIceBalls[i]->body->GetTransform().p.x), METERS_TO_PIXELS(listOfIceBalls[i]->body->GetTransform().p.y), isFlipped, &iceBallAnimations[i]->GetCurrentFrame(), zoomFactor);
				iceBallAnimations[i]->Update();
			}
			
		}
	}

	//Updating the iceBallsCounter if is less than the cooldown
	if (counterForIceBalls<playerCooldown) 
	{

		counterForIceBalls++;
	}

	return true;
}

bool Player::CleanUp()
{

	return true;
}


void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (physA->ctype == ColliderType::PLAYER) 
	{
		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if (isJumping && jumpingCounter > 1)
			{
				isJumping = false;

			}
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			if (!godMode)
			{
				lifes--;
				if (lifes < 1) {
					speed = 0;
					isDead = true;
				}
			}
			else
			{
				if (isJumping && jumpingCounter > 1)
				{
					isJumping = false;
				}
			}
			break;
		case ColliderType::ENEMY:
			LOG("Collision DEATH");
			if (!godMode)
			{
				lifes--;
				if (lifes < 1) {
					speed = 0;
					isDead = true;
				}
			}
			else
			{
				if (isJumping && jumpingCounter > 1)
				{
					isJumping = false;
				}
			}
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
		
	}
	else if (physA->ctype == ColliderType::PROYECTILE)
	{
		switch (physB->ctype)
		{
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			iceBallToDestroyIndex = listOfIceBalls.Find(physA);
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			iceBallToDestroyIndex = listOfIceBalls.Find(physA);
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::ENEMY:
			LOG("Collision UNKNOWN");
			break;

		}
	}
	
}