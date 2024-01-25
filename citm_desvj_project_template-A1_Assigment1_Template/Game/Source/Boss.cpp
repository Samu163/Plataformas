#include "Boss.h"
#include "app.h"
#include "textures.h"
#include "audio.h"
#include "input.h"
#include "render.h"
#include "scene.h"
#include "log.h"
#include "point.h"
#include "physics.h"
#include "map.h"
#include "pathfinding.h"


Boss::Boss() : Entity(EntityType::BOSS_ENEMY)
{
	name.Create("Boss");
}

Boss::~Boss()
{
	//if (receivedmg != nullptr)
	//	receivedmg->pendingtodelete = true;
	//if (afflictdmg != nullptr)
	//	afflictdmg->pendingtodelete = true;
}
bool Boss::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//Size of each cell of the spritesheet: 122x 91y


	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.PushBack({ 122,0, 122, 91 });
	walkAnim.PushBack({ 244, 0, 122, 91 });
	walkAnim.PushBack({ 366, 0, 122, 91 });
	walkAnim.PushBack({ 488, 0, 122, 91 });
	walkAnim.PushBack({ 610, 0, 122, 91 });
	walkAnim.PushBack({ 732, 0,122, 91 });
	walkAnim.PushBack({ 854,0, 122, 91 });

	walkAnim.speed = 0.2f;
	walkAnim.loop = true;
	
	

	attackAnim.PushBack({ 976, 0, 122, 91 });
	attackAnim.PushBack({ 1098, 0, 122, 91 });
	attackAnim.PushBack({ 1220, 0, 122, 91 });
	attackAnim.PushBack({ 1342, 0, 122, 91 });
	attackAnim.PushBack({ 1464, 0, 122, 91 });
	attackAnim.PushBack({ 1586, 0, 122, 91 });
	attackAnim.speed = 0.6f;
	attackAnim.loop = false;

	FollowAnim.PushBack({ 976, 0, 122, 91 });
	FollowAnim.PushBack({ 1098, 0, 122, 91 });
	FollowAnim.PushBack({ 1220, 0, 122, 91 });
	FollowAnim.PushBack({ 1342, 0, 122, 91 });
	FollowAnim.PushBack({ 1464, 0, 122, 91 });
	FollowAnim.PushBack({ 1586, 0, 122, 91 });
	FollowAnim.speed = 0.2f;
	FollowAnim.loop = true;


	deathAnim.PushBack({ 1708, 0, 122, 91 });
	deathAnim.PushBack({ 1830, 0, 122, 91 });
	deathAnim.PushBack({ 1952, 0, 122, 91 });
	deathAnim.PushBack({ 2074, 0, 122, 91 });


	deathAnim.speed = 0.2f;
	deathAnim.loop = false;

	isDeadAnim.PushBack({ 122, 642, 122, 107 });
	isDeadAnim.speed = 0.2f;
	isDeadAnim.loop = false;

	return true;

}

bool Boss::Start()
{
	//initilize textures
	texture = app->tex->Load("Assets/Textures/Boss.png");

	idleEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/idleEn1.ogg");
	attackEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/attackEn1.ogg");
	deathEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/deathEn1.ogg");


	//initialize player parameters
	Init();
	return true;
}

void Boss::Init()
{
	//Init function that initialize most of the player parameters
	speed = 3.0f;
	isDead = false;
	isDestroyed = false;
	isOnSceen = true;
	initialPosition = position;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 25, position.y + 25, 25, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	visionRange = 2;
	counterForPath = 0;
	walkingRange = 370;
	counterForDead = 0;
	hasDead = false;

}




bool Boss::Update(float dt)
{
	if (!isOnSceen) {
		enemyState = state::NO_ENEMY;
	}


	if (enemyState != state::DEATH && enemyState != state::NO_ENEMY && enemyState != state::ATTACK )
	{

		if (app->scene->player->position.x+20 > initialPosition.x - walkingRange &&
			app->scene->player->position.x-20 < initialPosition.x + walkingRange && 
			app->scene->player->position.y < position.y + visionRange * 3 &&
			app->scene->player->position.y > position.y - visionRange * 3 &&
			!app->scene->player->isDead)
		{
			enemyState = state::GO_TO_PLAYER;
		}
		else
		{
			FollowAnim.Reset();
			app->map->pathfinding->CreatePath({ 0,0 }, { 0,0 });
			enemyState = state::WALK;
			
			counterForPath = 0;
		}
	}
	
	

	vel = b2Vec2(0, -GRAVITY_Y);
	iPoint playerPosition = app->map->WorldToMap(app->scene->player->position.x,
		app->scene->player->position.y);
	iPoint enemyPosition = app->map->WorldToMap(position.x,
		position.y);
	iPoint initialEnemyPosition = app->map->WorldToMap(initialPosition.x,
		initialPosition.y);

	iPoint pos;
	b2Vec2 newPos;

	

	switch (enemyState)
	{
	case state::GO_TO_PLAYER:

		time++;
		if (!app->scene->player->isDead) 
		{
			currentAnimation = &FollowAnim;

			app->map->pathfinding->CreatePath(enemyPosition, playerPosition);
			path = app->map->pathfinding->GetLastPath();
			if (counterForPath < path->Count() - 1)
			{
				counterForPath += 1;
			}
			else
			{
				enemyState = state::IDLE;
				counterForPath = 0;
				break;
			}


			//150= 5 segundos 
			if (time < 150) {

				pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
				vel = app->scene->CheckTheMovementWithPath(pos, position);
				vel.x *= 0;
				vel.y = -GRAVITY_Y;
				isFlipped = app->scene->CheckVelocityForFlip(vel);
			}
			else {
				pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
				vel = app->scene->CheckTheMovementWithPath(pos, position);
				vel.x *= 4;
				vel.y = -GRAVITY_Y;
				isFlipped = app->scene->CheckVelocityForFlip(vel);


			}
			if (time >= 200) {
				time = 0;
			}
			FollowAnim.Update();
		}
		else
		{

			enemyState = state::IDLE;
			break;
		}
		app->audio->PlayFx(idleEn1Fx);
		break;
	case state::RETURNING_HOME:
		time = 0;
		currentAnimation = &walkAnim;
		app->map->pathfinding->CreatePath(enemyPosition, initialEnemyPosition);
		path = app->map->pathfinding->GetLastPath();
		if (counterForPath < path->Count())
		{
			counterForPath += 1;
		}
		else
		{
			enemyState = state::WALK;
			counterForPath = 0;
			break;
		}
		pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
		vel = app->scene->CheckTheMovementWithPath(pos, position);
		isFlipped = app->scene->CheckVelocityForFlip(vel);
		walkAnim.Update();
		app->audio->PlayFx(idleEn1Fx);
		break;
	case state::WALK:
		time = 0;
		currentAnimation = &walkAnim;
		if (position.x < initialPosition.x - walkingRange && speed < 0)
		{
			speed *= -1;
			position.x = initialPosition.x - walkingRange;
			break;
		}
		else if (position.x > initialPosition.x + walkingRange && speed > 0)
		{
			speed *= -1;
			position.x = initialPosition.x + walkingRange;
			break;
		}
		vel.x = speed;
		isFlipped = app->scene->CheckVelocityForFlip(vel);
		walkAnim.Update();
		//path.Update();
		app->audio->PlayFx(idleEn1Fx);
		break;
	case state::ATTACK:

		currentAnimation = &attackAnim;
		if (attackDuration > 50)
		{
			enemyState = state::IDLE;
			attackDuration = 0;
			attackAnim.Reset();

		}
		attackDuration++;
		attackAnim.Update();
		app->audio->PlayFx(attackEn1Fx);
		//path.Update();
		break;

	case state::DEATH:
		time = 0;
		vel = b2Vec2(0, -GRAVITY_Y);
		currentAnimation = &deathAnim;
		deathAnim.Update();
		counterForDead++;
		if (counterForDead == 1) {
			isDestroyed = true;
			app->physics->DestroyObject(pbody);
			deathPosition = position;

		}
		if (counterForDead > 50) {
			enemyState = state::NO_ENEMY;
			counterForDead = 0;
		}
		app->audio->PlayFx(deathEn1Fx);
		break;
	case state::NO_ENEMY:
		time = 0;
		if (!isOnSceen) {
			currentAnimation = &isDeadAnim;
		}
		else
		{
			currentAnimation = &deathAnim;

		}
		if (!hasDead) {
			if (!isDestroyed) {
				app->physics->DestroyObject(pbody);
				isDestroyed = true;
			}
			if (!isOnSceen) {
				
				currentAnimation = &isDeadAnim;
				position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
				position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
			}
			else
			{
				isOnSceen = false;
			}
			deathPosition = position;
			hasDead = true;
		}
		break;
	}
	if (enemyState != state::NO_ENEMY && enemyState != state::DEATH)
	{
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		//Draw texture
		app->render->DrawTexture(texture, position.x - 40, position.y - 45, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);

	}
	else
	{

		app->render->DrawTexture(texture, deathPosition.x - 40, deathPosition.y - 45, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);

	}
	app->scene->DrawPath();





	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::CHECKPOINT:
		LOG("Collision PLATFORM");
		physB->body->SetActive(false);

		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		enemyState = state::ATTACK;
		break;
	case ColliderType::PROYECTILE:
		LOG("Collision PLAYER");
		life--;
		if (life == 0) {
			speed = 0;
			enemyState = state::DEATH;
			deathTimer = 0;
		}
		break;

	}
}

