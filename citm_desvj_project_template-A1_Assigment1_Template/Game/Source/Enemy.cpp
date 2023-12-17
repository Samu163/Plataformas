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

	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.PushBack({ 0, 0, 122, 91 });
	walkAnim.speed = 0.2f;
	walkAnim.loop = true;

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

bool Enemy::Start()
{
	//initilize textures
	texture = app->tex->Load("Assets/Textures/Enemy.png");
	//initialize player parameters
	Init();
	return true;
}

void Enemy::Init()
{
	//Init function that initialize most of the player parameters
	speed = 3.0f;
	isDead = false;
	initialPosition = position;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	visionRange = 200;
	counterForPath = 0;
	walkingRange = 100;
}




bool Enemy::Update(float dt)
{

	if (enemyState != state::DEATH && enemyState != state::NO_ENEMY && enemyState != state::ATTACK )
	{

		if (app->scene->player->position.x+20 > initialPosition.x - walkingRange && app->scene->player->position.x-20 < initialPosition.x + walkingRange && !app->scene->player->isDead)
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
			vel.y = -GRAVITY_Y;
			isFlipped = app->scene->CheckVelocityForFlip(vel);


			walkAnim.Update();
		}
		else
		{
			enemyState = state::IDLE;
			break;
		}
		
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
		break;
	case state::ATTACK:
		currentAnimation = &attackAnim;
		if (attackDuration > 40)
		{
			enemyState = state::IDLE;
			attackDuration = 0;
		}
		attackDuration++;
		walkAnim.Update();
		//path.Update();
		break;

	case state::DEATH:
		vel = b2Vec2(0, -GRAVITY_Y);
		currentAnimation = &deathAnim;
		deathAnim.Update();
		break;
	case state::NO_ENEMY:
		currentAnimation = &deathAnim;
		if (hasDead) {
			app->physics->DestroyObject(pbody);
			hasDead = false;
		}
		break;
	}
	if (enemyState != state::NO_ENEMY)
	{
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		//Draw texture
		app->render->DrawTexture(texture, position.x - 40, position.y - 10, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);

	}
	else
	{
		app->render->DrawTexture(texture, deathPosition.x - 40, deathPosition.y - 37, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
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
		if (enemyState == state::DEATH) {
			deathPosition = position;
			enemyState = state::NO_ENEMY;
			hasDead = true;
		}
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		if (enemyState == state::DEATH) {
			deathPosition = position;
			enemyState = state::NO_ENEMY;
			hasDead = true;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		
		enemyState = state::ATTACK;
	case ColliderType::PROYECTILE:
		LOG("Collision PLAYER");
		life--;
		if (life < 1) {
			speed = 0;
			enemyState = state::DEATH;
			deathTimer = 0;
		}
		break;

	}
}

