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


Enemy::Enemy() : Entity(EntityType::ENEMY)
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
	isJumping = false;
	isDead = false;
	initialPosition = position;
	currentAnimation = &idleAnim;
	pbody = app->physics->CreateCircle(position.x + 20, position.y + 20, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	visionRange = 200;
	counterForPath = 0;
	walkingRange = 200;
}


bool Enemy::Update(float dt)
{

	if (zombieState != state::DEATH && zombieState != state::NO_ENEMY)
	{

		if (app->scene->player->position.x > position.x - visionRange && app->scene->player->position.x < position.x + visionRange ) {
			
				zombieState = state::IDLE;
		
		}
		else
		{
			zombieState = state::WALK;
			counterForPath = 0;
			app->map->pathfinding->CreatePath({0,0}, { 0,0 });
			isFollowing = false;
		}
	}
	

	b2Vec2 vel = b2Vec2(0, -0.165);
	iPoint playerPosition = app->map->WorldToMap(app->scene->player->position.x,
		app->scene->player->position.y);
	iPoint enemyPosition = app->map->WorldToMap(position.x,
		position.y);
	const DynArray<iPoint>* path;
	iPoint pos;
	b2Vec2 newPos;
	switch (zombieState)
	{
	case state::IDLE:
		currentAnimation = &idleAnim;
		app->map->pathfinding->CreatePath(enemyPosition, playerPosition);
		path = app->map->pathfinding->GetLastPath();
		if (counterForPath < path->Count()-1)
		{
			counterForPath += 0.1;
		}
		else
		{
			zombieState = state::ATTACK;
			counterForPath = 0;
			break;
		}
		pos = app->map->MapToWorld(path->At(counterForPath)->x, path->At(counterForPath)->y);
		newPos = b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y));
		pbody->body->SetTransform(newPos, 0);

		idleAnim.Update();
		break;
	case state::WALK:
		currentAnimation = &walkAnim;
		if (position.x < initialPosition.x - walkingRange && speed < 0)
		{
			speed *= -1;
			isFlipped = false;
			position.x = initialPosition.x - walkingRange;
			break;
		}
		else if (position.x > initialPosition.x + walkingRange && speed > 0)
		{
			speed *= -1;
			isFlipped = true;
			position.x = initialPosition.x + walkingRange;
			break;
		}
		vel = b2Vec2(speed, -0.165);
		walkAnim.Update();
		//path.Update();
		break;

	case state::DEATH:
		vel = b2Vec2(0, -GRAVITY_Y);
		currentAnimation = &deathAnim;
		deathTimer++;
		if (deathTimer > 20)
		{
			deathTimer = 0;
			app->physics->DestroyObject(pbody);
			currentAnimation = nullptr;
			zombieState = state::NO_ENEMY;
			break;
		}
	}

	if (currentAnimation != nullptr)
	{
		//Set the velocity of the pbody of the player
		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		if (zombieState != state::IDLE) 
		{
			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		}
		else
		{

		}
		

		//Draw texture
		app->render->DrawTexture(texture, position.x, position.y, isFlipped, &currentAnimation->GetCurrentFrame(), zoomFactor);
		app->scene->DrawPath();
	}





	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");

		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");

		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		life--;
		if (life < 1) {
			speed = 0;
			zombieState = state::DEATH;
			deathTimer = 0;
		}
	case ColliderType::PROYECTILE:
		LOG("Collision PLAYER");
		life--;
		if (life < 1) {
			speed = 0;
			zombieState = state::DEATH;
			deathTimer = 0;
		}
		break;

	}
}

