#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
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
	Animation headExplosion;
	Animation noHeadExplosion;
	Animation firstHit;
	Animation deathAnim;
	Animation* currentAnimation = nullptr;

	enum class state
	{
		IDLE,
		WALK,
		DEATH,
		NO_ENEMY

	};

	state zombieState = state::WALK;

	float speed = 0.4f;
	int visionRange;
	int walkingRange;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	int life = 2;

	int zoomFactor = 1.0f;
	//const int playerCooldown = 20;

	//Bools
	bool isDead;
	bool godMode;
	bool isJumping;
	bool isFlipped;
	bool isFollowing;

	//Counters
	int jumpingCounter;
	int doubleJumpCounter;

	//Death Counter
	int deathTimer = 0;

};
#endif // __ENEMY_H__