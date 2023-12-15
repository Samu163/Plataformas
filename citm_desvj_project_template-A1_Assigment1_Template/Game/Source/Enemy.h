#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "EntityManager.h"


#include "Entity.h"

struct SDL_Texture;
struct Collider;


class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	void Init();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	

public:

	//Path path;

	Animation idleAnim;
	Animation walkAnim;
	Animation attackAnim;
	Animation firstHit;
	Animation deathAnim;
	Animation* currentAnimation = nullptr;

	enum class state
	{
		IDLE,
		WALK,
		GO_TO_PLAYER,
		RETURNING_HOME,
		ATTACK,
		DEATH,
		NO_ENEMY
		

	};

	state flyingEnemyState = state::WALK;

	float speed = 0.4f;
	int visionRange;
	int walkingRange;
	int attackRange;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;



	int zoomFactor = 1.0f;
	//const int playerCooldown = 20;

	//Bools
	bool isDead;
	bool godMode;
	bool isJumping;
	bool isFlipped;
	bool isFollowing;

	//Counters
	float counterForPath;
	int doubleJumpCounter;

	//Death Counter
	int deathTimer = 0;
	int attackDuration = 0;


private:

	int life = 4;
	const DynArray<iPoint>* path;
};
#endif // __ENEMY_H__