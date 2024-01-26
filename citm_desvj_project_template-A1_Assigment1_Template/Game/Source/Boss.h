#ifndef __BOSS_H__
#define __BOSS_H__

#include "Point.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "EntityManager.h"
#include "physics.h"


#include "Entity.h"

struct SDL_Texture;
struct Collider;


class Boss : public Entity
{
public:

	Boss();

	virtual ~Boss();

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
	Animation FollowAnim;
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

	int cowFx;
	int tickingFx;
	int deathEn1Fx;
	bool Isattacking =false;
	bool Isattacking2 = false;
	float speed = 0.4f;
	int visionRange;
	int walkingRange;
	int attackRange;
	int time = 0.0f;
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
	int life = 10;

	bool hasDead = false;

private:

	iPoint deathPosition;

	const DynArray<iPoint>* path;
	b2Vec2 vel;
	int counterForDead = 0;

};
#endif // __ENEMY_H__