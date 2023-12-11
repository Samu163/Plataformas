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

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation idleAnim;
	Animation walkAnim;
	Animation jumpAnim;
	Animation hitAnim;
	Animation deathAnim;
	Animation* currentAnimation = nullptr;

	float speed = 0.4f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* iceBallTexture = NULL;
	PhysBody* pbody;

	List<PhysBody*> listOfIceBalls;
	List<PhysBody*> listOfIceBallsToDestroy;

	List<Animation*> iceBallAnimations;

	int pickCoinFxId;
	int lifes = 1;
	int zoomFactor = 1.0f;
	int iceBallToDestroyIndex = -1;
	const int playerCooldown = 20;

	//Bools
	bool isDead;
	bool godMode;
	bool isJumping;
	bool isFlipped;

	//Counters
	int jumpingCounter;
	int doubleJumpCounter;
	int counterForIceBalls;

};
#endif // __ENEMY_H__