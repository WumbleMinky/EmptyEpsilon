#include "playerinfo.h"
#include "spaceObjects/playerSpaceship.h"
#include "systemsPresetControls.h"

#include "gui/gui2_button.h"
#include "gui/gui2_slider.h"
#include "gui/gui2_label.h"

GuiSystemsPresetControls::GuiSystemsPresetControls(GuiContainer* owner, string id, ESystem* selected_system, GuiSlider* coolant_slider, GuiSlider* power_slider)
: GuiAutoLayout(owner, id, LayoutVerticalBottomToTop)
{
    //The selected_system, coolant_slider, and power_slider are passed so that the class can update the sliders' values if
    // a preset button is pressed and the selected_system's values change.
	setSize(50, GuiElement::GuiSizeMax);

	buttons.resize(NUM_OF_BUTTONS);
	presets.resize(NUM_OF_BUTTONS);

	for(int n = NUM_OF_BUTTONS - 1; n >= 0; n--)
	{
		buttons[n] =  new GuiButton(this, id + "_" + string(n) + "_PRESET_BUTTON", string(n+1), [this,n,selected_system,power_slider,coolant_slider](){
			if (this->edit_button->getText()=="Cancel")
            {
                if(!presets[n].empty())  //overwrite the settings.
                {
                    presets[n].clear();
                }
                //save the settings for every system in the presets vector.
                for(int i = 0; i < SYS_COUNT; i++)
                {
                    SystemSetting sys;
                    sys.system = ESystem(i);
                    sys.power_level = my_spaceship->systems[i].power_request;
                    sys.coolant_level = my_spaceship->systems[i].coolant_request;
                    presets[n].push_back(sys);
                }
                this->toggleEditButton();
            }
            else
            {
                for (int i = 0; i < SYS_COUNT; i++)
                {
                    SystemSetting sys = presets[n][i];
                    my_spaceship->commandSetSystemPowerRequest(sys.system, sys.power_level);
                    my_spaceship->commandSetSystemCoolantRequest(sys.system, sys.coolant_level);
                    if (sys.system == *selected_system)
                    {
                        coolant_slider->setValue(sys.coolant_level);
                        power_slider->setValue(sys.power_level);
                    }
                }
            }

		});
		buttons[n]->setSize(50, 50);
		buttons[n]->disable();
	}

	edit_button = new GuiButton(this, id + "_EDIT_BUTTON", "Save", [this](){
        this->toggleEditButton();
    });
    edit_button->setSize(50,50)->setPosition(0, 0, ACenter)->setMargins(0,0,0,0);
    edit_button->setTextSize(20);
    edit_button->enable();

    preset_label = new GuiLabel(this, id + "_LABEL", "Presets", 40);
    preset_label->setVertical()->setSize(50, 120)->setPosition(0, 0, ATopCenter);
}

//toggle function that is used by the edit and preset buttons. Enables/disables the preset buttons and change the edit button text.
void GuiSystemsPresetControls::toggleEditButton()
{
    //Enable the preset buttons in order to save a setting.
    if (this->edit_button->getText()=="Save")
    {
        for(int i = 0; i < this->NUM_OF_BUTTONS; i++)
        {
            this->buttons[i]->enable();
        }
        this->edit_button->setText("Cancel");
    }
    else //disable the preset buttons unless they have a preset saved for them.
    {
        for(int i=0; i < this->NUM_OF_BUTTONS; i++)
        {
            if(this->presets[i].empty())
                this->buttons[i]->disable();
        }
        this->edit_button->setText("Save");
    }
}
