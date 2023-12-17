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
#include "EnemyFlying.h"


FlyingEnemy::FlyingEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("Enemy");
}

FlyingEnemy::~FlyingEnemy()
{
}
bool FlyingEnemy::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//Size of each cell of the spritesheet: 122x 91y
	//Size of each cell of the spritesheet (Enemy): 72x 59y
	idleAnim.PushBack({ 0, 0, 122, 91 });
	idleAnim.PushBack({ 122, 0, 122, 91 });
	idleAnim.PushBack({ 244, 0, 122, 91 });
	idleAnim.speed = 0.2f;
	idleAnim.loop = true;

	walkAnim.PushBack({366, 0, 122, 91 });
	walkAnim.PushBack({ 0, 91, 122, 91 });
	walkAnim.PushBack({ 122, 91, 122, 91 });
	walkAnim.speed = 0.2f;
	walkAnim.loop = true;
	
	attackAnim.PushBack({244, 91, 122, 91 });
	attackAnim.PushBack({ 366, 91, 122, 91 });
	attackAnim.PushBack({ 0, 182, 122, 91 });
	attackAnim.PushBack({ 122, 182, 122, 91 });
	attackAnim.PushBack({ 244, 182, 122, 91 });
	attackAnim.PushBack({ 366, 182, 122, 91 });
	attackAnim.PushBack({ 0, 273, 122, 91 });
	attackAnim.speed = 0.2f;
	attackAnim.loop = true;

	deathAnim.PushBack({ 122, 273, 122, 91 });
	deathAnim.PushBack({ 244, 273, 122, 91 });
	deathAnim.PushBack({ 366, 273, 122, 91 });
	deathAnim.PushBack({ 0, 364, 122, 91 });


	deathAnim.speed = 0.2f;
	deathAnim.loop = false;

	return true;

}

bool FlyingEnemy::Start()
{
	//initilize textures
	texture = app->tex->Load("Assets/Textures/Enemy1.png");
	//initialize player parameters
	Init();
	return true;
}

void FlyingEnemy::Init()
{
	//Init function that initialize most of the player parameters
	speed = 3.0f;
	hasDead = false;
	initialPosition = position;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 25, position.y + 25, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	visionRange = 200;
	counterForPath = 0;
	walkingRange = 200;
}




bool FlyingEnemy::Update(float dt)
{
	//Checking if the enemy is dead
	if (flyingEnemyState != state::DEATH && flyingEnemyState != state::NO_ENEMY && flyingEnemyState != state::ATTACK )
	{
		//Checking if the player is on range
		if (app->scene->player->position.x > position.x - visionRange && app->scene->player->position.x < position.x + visionRange && !app->scene->player->isDead)
		{
				flyingEnemyState = state::GO_TO_PLAYER;
		}
		else
		{
			//if is out of his position, return to the initial position
			if (position.y> initialPosition.y+50 || position.y < initialPosition.y - 50) 
			{
				flyingEnemyState = state::RETURNING_HOME;
			}
			else
			{
				if (flyingEnemyState == state::GO_TO_PLAYER || flyingEnemyState == state::RETURNING_HOME) {
					app->map->pathfinding->CreatePath({ 0,0 }, { 0,0 });

				}
				flyingEnemyState = state::WALK;
				
			}
			//Updating the counter for drawing the path
			counterForPath = 0;
		}
	}
	
	
	//Init some variables 
	b2Vec2 vel = b2Vec2(0, -0.165);
	iPoint playerPosition = app->map->WorldToMap(app->scene->player->position.x,
		app->scene->player->position.y);
	iPoint enemyPosition = app->map->WorldToMap(position.x,
		position.y);
	iPoint initialEnemyPosition = app->map->WorldToMap(initialPosition.x,
		initialPosition.y);
	iPoint pos;
	b2Vec2 newPos;


	switch (flyingEnemyState)
	{
	case state::GO_TO_PLAYER:
		//checking if the player is dead
		if (!app->scene->player->isDead) 
		{
			currentAnimation = &walkAnim;
			//Creating a path from his position to the player
			app->map->pathfinding->CreatePath(enemyPosition, playerPosition);
			path = app->map->pathfinding->GetLastPath();
			if (counterForPath < path->Count() - 1)
			{
				counterForPath += 1;
			}
			else
			{
				flyingEnemyState = state::IDLE;
				counterForPath = 0;
				break;
			}
			pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
			//Checing his velocity and orientation
			vel = app->scene->CheckTheMovementWithPath(pos, position);
			isFlipped = app->scene->CheckVelocityForFlip(vel);

			walkAnim.Update();
		}
		else
		{
			flyingEnemyState = state::IDLE;
			break;
		}
		
		break;
	case state::RETURNING_HOME:
		currentAnimation = &walkAnim;
		//Creating a path to return to the initial position 
		app->map->pathfinding->CreatePath(enemyPosition, initialEnemyPosition);
		path = app->map->pathfinding->GetLastPath();
		if (counterForPath < path->Count()-1)
		{
			counterForPath += 1;
		}
		else
		{
			flyingEnemyState = state::WALK;
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
		vel = b2Vec2(speed, -0.165);
		walkAnim.Update();
		isFlipped = app->scene->CheckVelocityForFlip(vel);

		//path.Update();
		break;
	case state::ATTACK:
		currentAnimation = &attackAnim;
		if (attackDuration > 40)
		{
			flyingEnemyState = state::IDLE;
			attackDuration = 0;
		}
		attackDuration++;
		attackAnim.Update();
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
	if (flyingEnemyState != state::NO_ENEMY)
	{
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		//Draw texture
		app->render->DrawTexture(texture, position.x-40, position.y-10, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
		
	}
	else
	{
		app->render->DrawTexture(texture, deathPosition.x - 40, deathPosition.y-37, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
	}
	app->scene->DrawPath();




	return true;
}

void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (flyingEnemyState == state::DEATH) {
			deathPosition = position;
			flyingEnemyState = state::NO_ENEMY;
			hasDead=true;
		}
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		if (flyingEnemyState == state::DEATH) {
			deathPosition = position;
			flyingEnemyState = state::NO_ENEMY;
			hasDead = true;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		flyingEnemyState = state::ATTACK;
		break;
	case ColliderType::PROYECTILE:
		LOG("Collision PLAYER");
		life--;
		if (life < 1) {
			speed = 0;
			flyingEnemyState = state::DEATH;
			deathTimer = 0;
		}
		break;

	}
}

