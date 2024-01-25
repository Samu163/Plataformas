#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	PhysBody* pbody;
	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation pickedAnim;
	bool isCollected = false;
	int counterForAnim = 0;



private:
	bool isPickedRef = false;
	SDL_Texture* texture;
	const char* texturePath;
};

#endif // __ITEM_H__