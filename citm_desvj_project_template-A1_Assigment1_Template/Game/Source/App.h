#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"
#include "Player.h"
#include "Title.h"
#include "FadeToBlack.h"
#include "CreditsScene.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class EntityManager;
class Map;
class Physics;
class GuiManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;


	void LoadRequest();
	void SaveRequest();


private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// L14: TODO 1: Declare the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame

	
	

	void LoadFromFile();

	void SaveFromFile();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	EntityManager* entityManager;
	Map* map;
	Physics* physics;
	Player* player;
	GuiManager* guiManager;
	Title* titleScreen;
	ModuleFadeToBlack* fadeToBlack;
	Credits* credits;
	Win* winscene;
	Lose* loseScene;

	bool isIn30fps;



private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	uint32 maxFrameDuration = 16;


	bool saveRequest;
	bool loadRequest;

};

extern App* app;

#endif	// __APP_H__