#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class CheckPoint : public Entity
{
public:

	CheckPoint();
	virtual ~CheckPoint();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void CheckPoint::OnCollision(PhysBody* physA, PhysBody* physB);


public:

	bool isPicked = false;

	PhysBody* pbody;
	int counter;
	int checkPointFxId;


private:

	SDL_Texture* texture;
	Animation* currentAnimation = nullptr;
	const char* texturePath;
	Animation upAnim;
	Animation downAnim;
	Animation risingAnim;


	bool isPickedRef = false;


};

#endif // __ITEM_H__