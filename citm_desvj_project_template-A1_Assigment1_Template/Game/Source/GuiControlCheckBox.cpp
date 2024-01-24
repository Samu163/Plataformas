#include "GuiControlCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

// Constructor
GuiControlCheckBox::GuiControlCheckBox(uint32 id, SDL_Rect bounds, const char* text)
    : GuiControl(GuiControlType::CHECKBOX, id), checked(false)
{
    this->bounds = bounds;
    this->text = text;
}

// Destructor
GuiControlCheckBox::~GuiControlCheckBox()
{

}

// Update method
bool GuiControlCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check if the mouse is over the CheckBox
        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w &&
            mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
        }
    }

    //Render
    app->render->DrawRectangle(bounds, 0, 255, 255, 255, true, false);
    if (checked)
    {
        // Draw something to indicate the CheckBox is checked (like a checkmark or filled box)
        app->render->DrawRectangle({ bounds.x + 4, bounds.y + 4, bounds.w - 8, bounds.h - 8 }, 0, 255, 0, 255, true, false);
    }

    // Optional: Draw the text label for the CheckBox
    app->render->DrawText(text.GetString(), bounds.x - bounds.w -10, bounds.y, 150, bounds.h);

    return false;
}

void GuiControlCheckBox::SetChecked(bool checked)
{
    this->checked = checked;
}

bool GuiControlCheckBox::IsChecked() const
{
    return checked;
}
