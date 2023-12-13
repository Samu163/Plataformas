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

struct IceBall 
{
	PhysBody* iceBallCollider = new PhysBody();
	bool pendingToDelete = false;
	int counterForDelete = 0;
	Animation* currentIceBallAnimation = nullptr;
	int x, y;
};



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

	void OnCollision(PhysBody* physA, PhysBody* physB);


private: 
	Animation idleAnim;
	Animation walkAnim;
	Animation rightAnim;
	Animation jumpAnim;
	Animation deathAnim;
	Animation deathIceBallAnim;
	Animation iceBallAnim;

public:

	
	Animation* currentAnimation = nullptr;
	

	float speed = 0.4f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* iceBallTexture = NULL;
	PhysBody* pbody;

	List<IceBall> listOfIceBalls;
	//List<IceBall> listOfIceBallsToDestroy;

	int pickCoinFxId;
	int lifes = 1;
	int zoomFactor = 1.0f;
	const int playerCooldown = 10;

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

#endif // __PLAYER_H__