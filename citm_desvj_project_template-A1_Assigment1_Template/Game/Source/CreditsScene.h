#ifndef __CREDITSSCENE_H__
#define __CREDITSSCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
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

class Credits : public Module
{
public:

	Credits();

	// Destructor
	virtual ~Credits();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);


public: 
	
	SDL_Texture* windowTex = nullptr;

	uint windowW, windowH;

	int counter;


private:


	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;


	bool isInDebugMode = false;

	GuiControlButton* exitPauseButton;
	GuiControlButton* creditsButton;
	GuiControlButton* startButton;
	GuiControlButton* continueButton;
	GuiControlButton* backSettingsButton;
	GuiControlButton* exitButton;
	GuiControlButton* settingsButton;
	GuiControlSlider* musicSlider;
	GuiControlSlider* fxSlider;
	GuiControlCheckBox* fullScreenBox;
	GuiControlCheckBox* vSyncBtn;


	bool isFullScreen = false;

};

#endif // __SCENE_H__