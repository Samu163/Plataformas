#include "enemy.h"
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


Enemy::Enemy() : Entity(EntityType::WALKING_ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy()
{
	//if (receivedmg != nullptr)
	//	receivedmg->pendingtodelete = true;
	//if (afflictdmg != nullptr)
	//	afflictdmg->pendingtodelete = true;
}
bool Enemy::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//Size of each cell of the spritesheet: 122x 91y
	//Size of each cell of the spritesheet (Enemy): 72x 59y
	idleAnim.PushBack({ 0, 0, 122, 107 });
	idleAnim.PushBack({ 122, 0, 122, 107 });
	idleAnim.PushBack({ 244, 0, 122, 107 });
	idleAnim.speed = 0.2f;
	idleAnim.loop = true;

	walkAnim.PushBack({ 244, 214, 122, 107 });
	walkAnim.PushBack({ 0, 321, 122, 107 });
	walkAnim.PushBack({ 122, 321, 122, 107 });
	walkAnim.PushBack({ 244, 321, 122, 107 });
	walkAnim.PushBack({ 0, 428, 122, 107 });
	walkAnim.PushBack({ 122, 428, 122, 107 });
	walkAnim.speed = 0.2f;
	walkAnim.loop = true;
	
	
	attackAnim.PushBack({ 0, 107, 122, 107 });
	attackAnim.PushBack({ 122, 107, 122, 107 });
	attackAnim.PushBack({ 244, 107, 122, 107 });
	attackAnim.PushBack({ 0, 214, 122, 107 });
	attackAnim.PushBack({ 122, 214, 122, 107 });
	attackAnim.speed = 0.2f;
	attackAnim.loop = false;

	deathAnim.PushBack({ 244, 428, 122, 107 });
	deathAnim.PushBack({ 0, 535, 122, 107 });
	deathAnim.PushBack({ 122, 535, 122, 107 });
	deathAnim.PushBack({ 244, 535, 122, 107 });
	deathAnim.PushBack({ 0, 642, 122, 107 });
	deathAnim.PushBack({ 122, 642, 122, 107 });
	deathAnim.speed = 0.2f;
	deathAnim.loop = false;

	isDeadAnim.PushBack({ 122, 642, 122, 107 });
	isDeadAnim.speed = 0.2f;
	isDeadAnim.loop = false;

	return true;

}

bool Enemy::Start()
{
	//initilize textures
	texture = app->tex->Load("Assets/Textures/Enemy2.png");

	idleEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/idleEn1.ogg");
	attackEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/attackEn1.ogg");
	deathEn1Fx = app->audio->LoadFx("Assets/Audio/Fx/deathEn1.ogg");


	//initialize player parameters
	Init();
	return true;
}

void Enemy::Init()
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
	visionRange = 200;
	counterForPath = 0;
	walkingRange = 130;
	counterForDead = 0;
	hasDead = false;
}




bool Enemy::Update(float dt)
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
		if (!app->scene->player->isDead) 
		{
			currentAnimation = &walkAnim;
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
			pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
			vel = app->scene->CheckTheMovementWithPath(pos, position);
			vel.x *= 2;
			vel.y = -GRAVITY_Y;
			isFlipped = app->scene->CheckVelocityForFlip(vel);


			walkAnim.Update();
		}
		else
		{
			enemyState = state::IDLE;
			break;
		}
		app->audio->PlayFx(idleEn1Fx);
		break;
	case state::RETURNING_HOME:
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
		app->render->DrawTexture(texture, position.x - 40, position.y - 60, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);

	}
	else
	{
		app->render->DrawTexture(texture, deathPosition.x - 40, deathPosition.y - 60, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
	}
	app->scene->DrawPath();





	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
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

