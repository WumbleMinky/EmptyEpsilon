#ifndef SYSTEMS_PRESET_CONTROLS_H
#define SYSTEMS_PRESET_CONTROLS_H

#include "gui/gui2_autolayout.h"
#include "shipTemplate.h"

class GuiButton;
class GuiSlider;
class GuiLabel;

class GuiSystemsPresetControls : public GuiAutoLayout
{
private:
    const int NUM_OF_BUTTONS = 4;

    GuiLabel* preset_label;
    GuiButton* edit_button;
    std::vector<GuiButton*> buttons;

	struct SystemSetting {
		ESystem system;
		float power_level;
		float coolant_level;
	};
	std::vector< std::vector<SystemSetting> > presets;
public:
	GuiSystemsPresetControls(GuiContainer* owner, string id, ESystem* selected_system, GuiSlider* coolant_slider, GuiSlider* power_slider);
private:
    void toggleEditButton();
};
#endif//SYSTEMS_PRESET_CONTROLS_H
