#ifndef __GUICONTROLCHECKBOX_H__
#define __GUICONTROLCHECKBOX_H__

#include "GuiControl.h"
#include "SDL/include/SDL.h"

class GuiControlCheckBox : public GuiControl
{
public:
    GuiControlCheckBox(uint32 id, SDL_Rect bounds, const char* text);

    virtual ~GuiControlCheckBox();

    bool Update(float dt) override;

    void SetChecked(bool checked);
    bool IsChecked() const;

protected:
    bool checked; // Indicates whether the CheckBox is checked or not

    // Other properties and methods specific to the CheckBox control...
};

#endif // __GUICONTROLCHECKBOX_H__
