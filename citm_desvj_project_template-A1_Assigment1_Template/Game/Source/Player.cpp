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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	/*lifesAnim[0].PushBack({ 14, 22, 63, 33 });
	lifesAnim[1].PushBack({ 14, 106, 63, 33 });
	lifesAnim[2].PushBack({ 14, 186, 63, 33 });*/

	return true;
}

bool Player::Start()
{
	jumpingCounter = 0;
	isJumping = false;
	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN ) 
	{
		isJumping = true;
	

	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			vel = b2Vec2(-speed * dt * 2, -GRAVITY_Y);
		}
		else
		{
			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
			vel = b2Vec2(speed * dt*2, -GRAVITY_Y);
		}
		else
		{
			vel = b2Vec2(speed * dt, -GRAVITY_Y);
		}
		
	}

	if (isJumping) {
		

		int posYExtra = GRAVITY_Y/2 +jumpingCounter - dt;
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
		jumpingCounter = 0;
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

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
		if (isJumping) 
		{
			isJumping = false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}