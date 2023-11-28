#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "EntityManager.h"

#include "Entity.h"

struct SDL_Texture;
struct Collider;

class EnemyWalking : public Entity
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	EnemyWalking(int x, int y);

	void Init();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	// Sets flag for deletion and for the collider aswell
	virtual void SetToDelete();

public:
	// The current position in the world
	iPoint position;

	// The enemy's texture
	SDL_Texture* texture = nullptr;

	// Sound fx when destroyed
	int destroyedFx = 0;

	// A flag for the enemy removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	// The enemy's collider
	PhysBody* enemyBody;
	PhysBody* punchAttack = nullptr;

protected:
	// A ptr to the current animation
	Animation* currentAnim = nullptr;

	// Variable to know which enemy is

	int EnemyType = 0;

	

	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;
};

#endif // __ENEMY_H__