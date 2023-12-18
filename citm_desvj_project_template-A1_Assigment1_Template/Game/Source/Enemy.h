#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "physics.h"


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
	Animation isDeadAnim;
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

	state enemyState = state::WALK;

	int idleEn1Fx;
	int attackEn1Fx;
	int deathEn1Fx;

	float speed = 0.4f;
	int visionRange;
	int walkingRange;
	int attackRange;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	int zoomFactor = 1.0f;

	//Bools
	bool isDead;
	bool godMode;
	bool isJumping;
	bool isFlipped;
	bool isFollowing;
	bool isDestroyed = false;

	//Counters
	float counterForPath;
	int doubleJumpCounter;

	//Death Counter

	int deathTimer = 0;
	int attackDuration = 0;

	bool isOnSceen = true;

private:

	iPoint deathPosition;
	int life = 2;
	const DynArray<iPoint>* path;
	b2Vec2 vel;
	int counterForDead = 0;
	bool hasDead = false;

};
#endif // __ENEMY_H__