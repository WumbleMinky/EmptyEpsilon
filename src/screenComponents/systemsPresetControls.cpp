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
                if(!presets[n].empty())
                {
                    presets[n].clear();
                }
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
    edit_button->setSize(50,50)->setPosition(0, 0, ACenter)->setMargins(0,-20,0,20);
    edit_button->setTextSize(20);
    edit_button->enable();
    preset_label = new GuiLabel(this, id + "_LABEL", "Presets", 40);
    preset_label->setVertical()->setSize(50, 150)->setPosition(0, 0, ATopCenter);
}

void GuiSystemsPresetControls::toggleEditButton()
{
    if (this->edit_button->getText()=="Save")
    {

        for(int i = 0; i < this->NUM_OF_BUTTONS; i++)
        {
            this->buttons[i]->enable();
        }
        this->edit_button->setText("Cancel");
    }
    else
    {
        for(int i=0; i < this->NUM_OF_BUTTONS; i++)
        {
            if(this->presets[i].empty())
                this->buttons[i]->disable();
        }
        this->edit_button->setText("Save");
    }
}

void GuiSystemsPresetControls::onDraw(sf::RenderTarget& window)
{
	//Update the Save Button and Preset button based on whether there are settings saved in the presets vector
    if (!my_spaceship)
        return;

    for (int i = 0; i < NUM_OF_BUTTONS; i++)
    {
        if (presets[i].empty())
        {
            //buttons[i]->disable();
        }
        else
        {
            //buttons[i]->enable();
        }
    }
	GuiAutoLayout::onDraw(window);
}
