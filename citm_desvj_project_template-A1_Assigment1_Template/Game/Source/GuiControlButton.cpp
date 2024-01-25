#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "scene.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			break;
		case GuiControlState::NORMAL:
			//app->render->DrawRectangle(bounds, 0, 0, 255, 100, true, false);
			app->render->DrawTexture(texture, bounds.x - 40,bounds.y - 20,false,0,1,0, INT_MAX, INT_MAX,1,true);
			break;
		case GuiControlState::FOCUSED:
			app->render->DrawTexture(texture, bounds.x-40, bounds.y-20, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
			app->render->DrawRectangle(bounds, 0, 0, 20, 100, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawTexture(texture, bounds.x - 40, bounds.y-20, false, 0, 1, 0, INT_MAX, INT_MAX, 1, true);
			app->render->DrawRectangle(bounds, 0, 255, 0, 100, true, false);


			break;
		}

		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

