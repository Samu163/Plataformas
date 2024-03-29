#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Animation.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	physics = new Physics();
	scene = new Scene();
	map = new Map();
	entityManager = new EntityManager();
	player = new Player();
	guiManager = new GuiManager();
	titleScreen = new Title();
	credits = new Credits();
	fadeToBlack = new ModuleFadeToBlack(true);
	winscene = new Win();
	loseScene = new Lose();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(scene);
	AddModule(map);
	AddModule(entityManager);
	AddModule(fadeToBlack);
	AddModule(titleScreen);
	AddModule(winscene);
	AddModule(loseScene);

	AddModule(guiManager);
	AddModule(credits);




	// Render last to swap buffer
	AddModule(render);


	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	Timer timer = Timer();

	bool ret = LoadConfig();

	if(ret == true)
	{
		title = configNode.child("app").child("title").child_value(); 
		win->SetTitle(title.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	OPTICK_EVENT();

	bool ret = true;
	PrepareUpdate();
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (isIn30fps) {
			maxFrameDuration = 16;
			isIn30fps = false;
		}
		else
		{
			maxFrameDuration = 32;
			isIn30fps = true;
		}
	}
	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();


	OPTICK_FRAME("Main Loop");

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	OPTICK_EVENT();

	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_EVENT();

	// This is a good place to call Load / Save functions
	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32) (maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}

	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	
	// Amount of ms took the last update (dt)
	dt = (float) frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount; 
		lastSecFrameCount = 0;
	}


	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
		averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

	//Call load 
	if (loadRequest) {
		LoadFromFile();
		loadRequest = false;
	}
	//Call save From File
	if (saveRequest) {
		SaveFromFile();
		saveRequest = false;
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	OPTICK_EVENT();

	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadRequest()
{
	loadRequest = true;
};
void App::SaveRequest()
{
	saveRequest = true;
};

void App::LoadFromFile() 
{
	//Open the XML for reading 
	pugi::xml_document saveGameDoc;
	pugi::xml_parse_result result = saveGameDoc.load_file("save_game.xml");


	if (result) 
	{
		//Iterate all modules and get child of the module and call the LoadState()
		
		ListItem<Module*>* moduleItem;
		moduleItem = modules.start;


		while (moduleItem != NULL)
		{
			//Call the loadState() of the odule, passing as a parameter the XML node of the module

			moduleItem->data->LoadState(saveGameDoc.child("game_state").child(moduleItem->data->name.GetString()));
			moduleItem = moduleItem->next;
		}
	}
	else
	{
		LOG("Error Loading save_game.xml:  %s", result.description());
	}

	


}
void App::SaveFromFile() 
{
	//Open the XML for reading 
	pugi::xml_document saveGameDoc;
	pugi::xml_node gameState = saveGameDoc.append_child("game_state");

	//Iterate all modules and get child of the module and call the LoadState()

	ListItem<Module*>* moduleItem;
	moduleItem = modules.start;


	while (moduleItem != NULL)
	{
		//Create the module Element 
		pugi::xml_node moduleNode = gameState.append_child(moduleItem->data->name.GetString());


		//Call the loadState() of the odule, passing as a parameter the XML node of the module

		moduleItem->data->SaveState(moduleNode);
		moduleItem = moduleItem->next;
	}

	saveGameDoc.save_file("save_game.xml");

}



