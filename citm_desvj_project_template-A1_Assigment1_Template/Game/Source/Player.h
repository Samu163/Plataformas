#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;


class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	Animation idleAnim;
	Animation leftAnim;
	Animation rightAnim;
	Animation jumpAnim;
	Animation* currentAnimation = nullptr;

	float speed = 0.4f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	bool isJumping;
	int jumpingCounter;
	int pickCoinFxId;
	int doubleJumpCounter;
	bool isFlipped;
	int lifes = 1;
	bool isDead;
	bool godMode;

};

#endif // __PLAYER_H__