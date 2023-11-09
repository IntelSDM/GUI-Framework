#include "pch.h"
#include "entity.h"
#include "Form.h"
#include "TabController.h"
#include "Tab.h"
#include "Toggle.h"
#include "Label.h"
#include "Button.h"
#include "TextBox.h"
#include "Slider.h"
EntityVector MenuEntity;
int SelectedTab;
int TabCount;
bool ToggleTest;
bool ToggleTest1 = true;
std::string TextBoxText = "txtbox";
int intvalue = 3;
float floatvalue = 8.5f;
void CreateGUI()
{
	MenuEntity = std::make_shared< Container >();
	// We use the makeshared function instead of declaring new variables because they are automatically disposed

	MenuEntity->Draw();
	MenuEntity->Update();
}
void SetFormPriority()
{
	// This sorts the host container (containerptr) which contains forms, as long as a form isn't parented to another form then this will allow it to draw over when clicked.
	// I swear to god if i need to make this work for forms inside forms for some odd reason in the future then i am going to throw a monitor out the window.
	std::sort(MenuEntity->GetContainer().begin(), MenuEntity->GetContainer().end(),
		[](child a, child b) {return b->GetLastClick() < a->GetLastClick(); }
	);
}
void Render()
{
	MenuEntity->Draw();
	MenuEntity->GetContainer()[0]->Update(); // only allow stretching,dragging and other update stuff if it is the main form, prevents dragging and sizing the wrong forms.
	SetFormPriority();
}
