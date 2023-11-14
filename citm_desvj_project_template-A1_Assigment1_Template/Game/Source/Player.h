#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include "EntityManager.h"
#include <list>
#include <iostream>


using namespace std;
struct SDL_Texture;


class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	void Init();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void DestroyIceBall();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation idleAnim;
	Animation walkAnim;
	Animation rightAnim;
	Animation jumpAnim;
	Animation deathAnim;
	Animation* currentAnimation = nullptr;

	float speed = 0.4f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;

	List<PhysBody*> listOfIceBalls;

	bool destroyIceBall;
	bool isJumping;
	int jumpingCounter;
	int pickCoinFxId;
	int doubleJumpCounter;
	bool isFlipped;
	int lifes = 1;
	bool isDead;
	bool godMode;
	int zoomFactor = 1.0f;
	int iceBallToDestroy = -1;

};

#endif // __PLAYER_H__