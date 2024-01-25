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
	void ItemLives::OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPickedRef = false;
	bool isCollected = false;

	PhysBody* pbody;



private:

	SDL_Texture* texture;
	const char* texturePath;
	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation pickedAnim;
	int counterForAnim = 0;
};

#endif // __ITEM_H__