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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {
	//122x 91y
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
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

	leftAnim.PushBack({ 1098, 0, 122, 91 });
	leftAnim.PushBack({ 1220, 0, 122, 91 });
	leftAnim.PushBack({ 1342, 0, 122, 91 });
	leftAnim.PushBack({ 1464, 0, 122, 91 });
	leftAnim.PushBack({ 1586, 0, 122, 91 });
	leftAnim.speed = 0.2f;
	leftAnim.loop = true;

	jumpAnim.PushBack({ 122, 91, 122, 91 });
	jumpAnim.PushBack({ 244, 91, 122, 91 });
	jumpAnim.PushBack({ 366, 91, 122, 91 });
	jumpAnim.PushBack({ 488, 91, 122, 91 });
	jumpAnim.PushBack({ 610, 91, 122, 91 });
	jumpAnim.speed = 0.15f;
	jumpAnim.loop = false;
	

	return true;
}

bool Player::Start()
{
	jumpingCounter = 0;
	isJumping = false;
	//initilize textures
	texture = app->tex->Load("Assets/Textures/playerIce_.png");
	initialPosition = position;
	pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	currentAnimation = &idleAnim;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	//Debug
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		jumpingCounter = 0;
		isJumping = false;
		app->physics->DestroyObject(pbody);
		position = initialPosition;
		pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 18, bodyType::DYNAMIC);
		pbody->listener = this;
		pbody->ctype = ColliderType::PLAYER;
		currentAnimation = &idleAnim;
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}


	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	if (!isDead) 
	{

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			isJumping = true;
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			isFlipped = true;
			currentAnimation = &leftAnim;
			leftAnim.Update();

			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFlipped = false;
			currentAnimation = &leftAnim;
			leftAnim.Update();

			vel = b2Vec2(speed * dt, -GRAVITY_Y);
		}
		else
		{
			currentAnimation = &idleAnim;
			idleAnim.Update();
		}

		if (isJumping) {

			currentAnimation = &jumpAnim;
			jumpAnim.Update();
			LOG("%d", jumpingCounter);
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
		currentAnimation = &idleAnim;
		idleAnim.Update();
	}
	

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	
	app->render->DrawTexture(texture, position.x-45, position.y-40, isFlipped ,&currentAnimation->GetCurrentFrame());

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (isJumping&&jumpingCounter>1) 
		{
			isJumping = false;
			
		}
		break;
	case ColliderType::DEATH:
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