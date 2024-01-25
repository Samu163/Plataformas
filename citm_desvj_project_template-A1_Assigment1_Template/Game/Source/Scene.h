#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "EnemyFlying.h"
#include "Item.h"
#include "CheckPoint.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "GuiControlValueBox.h"
#include "GuiControlCheckBox.h"
#include "ItemLives.h"

#include <list>


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DrawPath();

	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

	bool OnGuiMouseClickEvent(GuiControl* control);

	//Just for checking the movement in other places (enmey, player) 
	b2Vec2 CheckTheMovementWithPath(iPoint positionOfThePath, iPoint originalPosition);

	bool CheckVelocityForFlip(b2Vec2 vel);
	void ShowPauseButtons(bool condition);
	void ShowSettings(bool condition);


public: 
	Player* player;
	Enemy* walkingEnemy_1;
	Enemy* walkingEnemy_2;
	Boss* boss;
	FlyingEnemy* flyingEnemy_1;
	FlyingEnemy* flyingEnemy_2;
	SString path;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* windowTex = nullptr;
	Animation pathAnim;
	iPoint mousePos;
	iPoint mouseTile;
	bool sameGame = false;


	List<CheckPoint*> listOfCheckPoints;
	List<Item*> listOfCoins;
	List<ItemLives*> listOfItemLives;

	bool retButton = true;

	bool isOnPause = false;

	uint windowW, windowH;


private:


	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;


	bool isInDebugMode = false;

	GuiControlButton* exitPauseButton;
	GuiControlButton* backSettingsButton;
	GuiControlButton* returnTitleButton;
	GuiControlButton* resumeButton;
	GuiControlButton* exitButton;
	GuiControlButton* settingsPauseButton;
	GuiControlSlider* musicSlider;
	GuiControlSlider* fxSlider;
	GuiControlCheckBox* fullScreenBox;
	GuiControlCheckBox* vSyncBtn;
	GuiControlValueBox* playerLifesBox;
	GuiControlValueBox* coinsBox;


	bool isFullScreen = false;

};

#endif // __SCENE_H__