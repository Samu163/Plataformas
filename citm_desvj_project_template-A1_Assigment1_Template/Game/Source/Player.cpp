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
#include "Scene.h"

#include "Window.h"


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
	
	attackAnim1.PushBack({ 0, 273, 122, 91 });
	attackAnim1.PushBack({ 122, 273, 122, 91 });
	attackAnim1.PushBack({ 244, 273, 122, 91 });
	attackAnim1.PushBack({ 366, 273, 122, 91 });
	attackAnim1.speed = 0.5f;
	attackAnim1.loop = false;

	attackAnim2.PushBack({ 488, 273, 122, 91 });
	attackAnim2.PushBack({ 610, 273, 122, 91 });
	attackAnim2.PushBack({ 732, 273, 122, 91 });
	attackAnim2.PushBack({ 854, 273, 122, 91 });
	attackAnim2.speed = 0.5f;
	attackAnim2.loop = false;

	attackAnim3.PushBack({ 976, 273, 122, 91 });
	attackAnim3.PushBack({ 1098, 273, 122, 91 });
	attackAnim3.PushBack({ 1220, 273, 122, 91 });
	attackAnim3.PushBack({ 1342, 273, 122, 91 });
	attackAnim3.speed = 0.5f;
	attackAnim3.loop = false;

	jumpAnim.PushBack({ 122, 91, 122, 91 });
	jumpAnim.PushBack({ 244, 91, 122, 91 });
	jumpAnim.PushBack({ 366, 91, 122, 91 });
	jumpAnim.PushBack({ 488, 91, 122, 91 });
	jumpAnim.PushBack({ 610, 91, 122, 91 });
	jumpAnim.speed = 0.15f;
	jumpAnim.loop = false;

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




	iceBallAnim.PushBack({ 0, 0, 45, 45 });
	iceBallAnim.PushBack({ 45, 0, 45, 45 });
	iceBallAnim.PushBack({ 90, 0, 45, 45 });
	iceBallAnim.PushBack({ 0, 45, 45, 45 });
	iceBallAnim.PushBack({ 45, 45, 45, 45 });
	iceBallAnim.PushBack({ 90, 45, 45, 45 });
	iceBallAnim.PushBack({ 0, 90, 45, 45 });
	iceBallAnim.PushBack({ 45, 90, 45, 45 });
	iceBallAnim.PushBack({ 90, 90, 45, 45 });
	iceBallAnim.PushBack({ 0, 135, 45, 45 });
	iceBallAnim.PushBack({ 45, 135, 45, 45 });
	iceBallAnim.PushBack({ 90, 135, 45, 45 });
	iceBallAnim.speed = 0.2f;
	iceBallAnim.loop = true;

	deathIceBallAnim.PushBack({ 0, 180, 45, 45 });
	deathIceBallAnim.PushBack({ 45, 180, 45, 45 });
	deathIceBallAnim.PushBack({ 90, 180, 45, 45 });
	deathIceBallAnim.PushBack({ 0, 225, 45, 45 });
	deathIceBallAnim.PushBack({ 45, 225, 45, 45 });
	deathIceBallAnim.PushBack({ 90, 225, 45, 45 });
	deathIceBallAnim.speed = 0.2f;
	deathIceBallAnim.loop = false;



	return true;
}

bool Player::Start()
{
	
	

	//initilize textures
	texture = app->tex->Load("Assets/Textures/playerIce.png");
	iceBallTexture = app->tex->Load("Assets/Textures/iceBall.png");
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/coinFx.ogg");
	
	jumpFx= app->audio->LoadFx("Assets/Audio/Fx/jump.ogg");
	/*runningFx= app->audio->LoadFx("Assets/Audio/Fx/run.ogg");*/
	magicFx1 = app->audio->LoadFx("Assets/Audio/Fx/magic1.ogg");
	magicFx2 = app->audio->LoadFx("Assets/Audio/Fx/magic2.ogg");
	magicFx3 = app->audio->LoadFx("Assets/Audio/Fx/magic3.ogg");
	iceballdeathFx = app->audio->LoadFx("Assets/Audio/Fx/iceballdeath.ogg");
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/death.ogg");
	initialPosition = position;

	//initialize player parameters
	Init();
	return true;
}

void Player::Init() 
{
	//Init function that initialize most of the player parameters
	speed = 0.4f;
	jumpingCounter = 0;
	playerCooldown = 40;

	AttackAnimCounter = -1;

	attackCounter = 20;
	isJumping = false;
	isDead = false;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	if (lifes <= 0) {
		lifes = 3;
	}

	
}

bool Player::Update(float dt)
{
	if (position.x > 7500 && position.x < 8000)
	{
		b2Vec2 newPos = b2Vec2(PIXEL_TO_METERS(10175), PIXEL_TO_METERS(320));
		pbody->body->SetTransform(newPos, 0);
	}



	if (app->scene->isOnPause)
	{
		app->win->GetWindowSize(app->scene->windowW, app->scene->windowH);
		app->render->DrawTexture(app->scene->windowTex, (app->scene->windowW / 2)-185, (app->scene->windowH / 2)-300, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		lastCheckPoint = app->scene->listOfCheckPoints[0]->position;
		b2Vec2 newPos = b2Vec2(PIXEL_TO_METERS(lastCheckPoint.x), PIXEL_TO_METERS(lastCheckPoint.y));
		pbody->body->SetTransform(newPos, 0);

	}
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		lastCheckPoint = app->scene->listOfCheckPoints[1]->position;
		b2Vec2 newPos = b2Vec2(PIXEL_TO_METERS(lastCheckPoint.x), PIXEL_TO_METERS(lastCheckPoint.y));
		pbody->body->SetTransform(newPos, 0);

	}



	//Debug
	//Restart from initial position
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (isDead) 
		{
			deathCounter = 0;
			deathAnim.Reset();
		}
		else
		{
			app->physics->DestroyObject(pbody);
		}
		position = initialPosition;
		Init();
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN )
	{
		if (isDead) 
		{
			deathCounter = 0;
			deathAnim.Reset();
		}
		else
		{
			app->physics->DestroyObject(pbody);
		}
		position = lastCheckPoint;
		Init();
	}


	//GodMode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) 
	{
		if (godMode) {
			b2Vec2 newDebugPos = b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
			pbody->body->SetTransform(newDebugPos,0);
		}
		godMode = !godMode;
	}

	

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	if (!isDead) 
	{
		float Auxiliartiempo = 0;
		//counter

		if (app->scene->isOnPause == true) {
			
			Auxiliartiempo = fps;
			fps = Auxiliartiempo;
		}
		else {
			secondsCount = fps / 60;
			fps++;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !app->scene->isOnPause)
		{
			isJumping = true;
			//Sound Effect
		}
		else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && playerCooldown == 40) && !app->scene->isOnPause)
		{

			//Set counter to shoot to 0, in order to shoot again
			attackAnim1.Reset();
			attackAnim2.Reset();
			attackAnim3.Reset();
			attackCounter = 0;


			AttackAnimCounter++;

			playerCooldown = 0;

			//Sound Effect
		}
		
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !app->scene->isOnPause)
		{
			isFlipped = true;
			currentAnimation = &walkAnim;
			walkAnim.Update();

			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
			app->audio->PlayFx(runningFx);
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !app->scene->isOnPause) {
			isFlipped = false;
			currentAnimation = &walkAnim;
			walkAnim.Update();

			vel = b2Vec2(speed * dt, -GRAVITY_Y);
			app->audio->PlayFx(runningFx);
		}
		else
		{
			currentAnimation = &idleAnim;
			idleAnim.Update();
		}
		//Jump Function with dt (is not working with 30fps)
		if (isJumping) 
		{
			if (jumpingCounter == 0) {
				app->audio->PlayFx(jumpFx);

			}
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

		//Attack Animation
		if (attackCounter < 20)
		{

			switch (AttackAnimCounter)
			{
			case 0:
				currentAnimation = &attackAnim1;
				attackAnim1.Update();
				app->audio->PlayFx(magicFx1);
				break;
			case 1:
				currentAnimation = &attackAnim2;
				attackAnim2.Update();
				app->audio->PlayFx(magicFx2);
				break;
			case 2:
				currentAnimation = &attackAnim3;
				attackAnim3.Update();
				app->audio->PlayFx(magicFx3);
				break;


			default:
				break;
			}
			
			if (AttackAnimCounter > 2) {
				AttackAnimCounter = 0;
			}
			
			attackCounter++;

		

		}
	

		//Init the ice Ball
		if (playerCooldown<40)
		{
			//Create the ball on the first frame
			if (playerCooldown == 0) 
			{
				IceBall iceBall;
				b2Vec2 iceBallVel = b2Vec2(20, 0);
				iceBall.currentIceBallAnimation = &iceBallAnim;

				if (isFlipped)
				{
					iceBallVel = b2Vec2(-20, 0);
					iceBall.iceBallCollider = app->physics->CreateCircle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 30, METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10, 7, bodyType::DYNAMIC);
				}
				else
				{
					iceBall.iceBallCollider = app->physics->CreateCircle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) + 30, METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10, 7, bodyType::DYNAMIC);
				}
				iceBall.iceBallCollider->body->SetLinearVelocity(iceBallVel);
				iceBall.iceBallCollider->listener = this;
				iceBall.iceBallCollider->ctype = ColliderType::PROYECTILE;
				listOfIceBalls.Add(iceBall);
			}
			playerCooldown++;
		}

		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		if (godMode) {
			position.x = -app->render->camera.x+ app->render->camera.w/2;
			position.y = -app->render->camera.y+ app->render->camera.h/2;
		}
		else
		{
			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		}
	

		//Draw texture
		if (!app->scene->isOnPause) {
			app->render->DrawTexture(texture, position.x - 45, position.y - 40, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);

		}

	}
	else
	{

		//If is dead draw the final animation in the death position
		if (deathCounter == 0) {
			app->physics->DestroyObject(pbody);
		
			app->audio->PlayFx(deathFx);
			deathPosition.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			deathPosition.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
			
			
		}
		else if (deathCounter >60 && lifes>0)
		{
			deathCounter = -1;
			//Respawn function
			if (isDead)
			{
				deathAnim.Reset();
			}
			else
			{
				app->physics->DestroyObject(pbody);
			}
			position = lastCheckPoint;
			Init();
		}
		position.x = deathPosition.x;
		position.y = deathPosition.y;
	

		currentAnimation = &deathAnim;
		deathAnim.Update();
	
		deathCounter++;


		
		app->render->DrawTexture(texture, deathPosition.x - 45, deathPosition.y - 40, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
		



	}
	

	//Drawing IceBalls
	for (int i = 0; i < listOfIceBalls.Count(); i++)
	{
		if (listOfIceBalls[i].pendingToDelete) 
		{
			//Drawing dead IceBalls
			if (listOfIceBalls[i].counterForDelete < 50)
			{
				//Doing destroy animation
				if (listOfIceBalls[i].counterForDelete == 0) 
				{
					listOfIceBalls[i].x = listOfIceBalls[i].iceBallCollider->body->GetTransform().p.x;
					listOfIceBalls[i].y = listOfIceBalls[i].iceBallCollider->body->GetTransform().p.y;
					app->physics->DestroyObject(listOfIceBalls[i].iceBallCollider);

					app->audio->PlayFx(iceballdeathFx);

				}
				listOfIceBalls[i].currentIceBallAnimation = &deathIceBallAnim;
				app->render->DrawTexture(iceBallTexture, METERS_TO_PIXELS(listOfIceBalls[i].x), METERS_TO_PIXELS(listOfIceBalls[i].y), isFlipped, &listOfIceBalls[i].currentIceBallAnimation->GetCurrentFrame(), zoomFactor);
				listOfIceBalls[i].currentIceBallAnimation->Update();
				listOfIceBalls[i].counterForDelete++;
			}
			else
			{
				//has finished destroyed animation
				listOfIceBalls[i].currentIceBallAnimation->Reset();
				listOfIceBalls.Del(listOfIceBalls.At(i));

			}
		}
		else
		{
			//Drawing regular Iceballs
			listOfIceBalls[i].currentIceBallAnimation = &iceBallAnim;
			app->render->DrawTexture(iceBallTexture, METERS_TO_PIXELS(listOfIceBalls[i].iceBallCollider->body->GetTransform().p.x)-20, METERS_TO_PIXELS(listOfIceBalls[i].iceBallCollider->body->GetTransform().p.y)-20, isFlipped, &listOfIceBalls[i].currentIceBallAnimation->GetCurrentFrame(), zoomFactor);
			iceBallAnim.Update();
		}
			
	}
	currentPosition = position;
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
			physB->body->SetActive(false);
			coinCount++;
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::LIVES_ITEM:
			LOG("Collision LIFES_ITEM");
			physB->body->SetActive(false);
			lifes++;
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::CHECKPOINT:
			LOG("Collision checkPoint");
			physB->body->SetActive(false);
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
				speed = 0;
				isDead = true;
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
			LOG("Collision ENEMY");
			if (!godMode)
			{
				lifes--;
				speed = 0;
				isDead = true;
			
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
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			physB->body->SetActive(false);
		case ColliderType::LIVES_ITEM:
			LOG("Collision LIFES_ITEM");
			physB->body->SetActive(false);
			break;
		case ColliderType::CHECKPOINT:
			LOG("Collision checkPoint");
			physB->body->SetActive(false);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			for (int i = 0; i < listOfIceBalls.Count(); i++)
			{
				if (listOfIceBalls[i].iceBallCollider == physA) 
				{
					listOfIceBalls[i].pendingToDelete = true;
				}
			}
			
			break;
		case ColliderType::DEATH:
			LOG("Collision DEATH");
			for (int i = 0; i < listOfIceBalls.Count(); i++)
			{
				if (listOfIceBalls[i].iceBallCollider == physA)
				{
					listOfIceBalls[i].pendingToDelete = true;
				}
			}
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::ENEMY:
			LOG("Collision ENEMY");
			for (int i = 0; i < listOfIceBalls.Count(); i++)
			{
				if (listOfIceBalls[i].iceBallCollider == physA)
				{
					listOfIceBalls[i].pendingToDelete = true;
				}
			}
			break;

		}
	}
	
}