#ifndef __ITEMLIVES_H__
#define __ITEMLIVES_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ItemLives : public Entity
{
public:

	ItemLives();
	virtual ~ItemLives();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPickedRef = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;
	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation pickedAnim;
	bool isCollected = false;
	int counterForAnim = 0;
};

#endif // __ITEM_H__