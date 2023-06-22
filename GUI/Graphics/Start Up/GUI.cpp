#include "pch.h"
#include "entity.h"
#include "Form.h"
#include "TabController.h"
#include "Tab.h"
EntityVector MenuEntity;
int SelectedTab;
int TabCount;
void CreateGUI()
{
	MenuEntity = std::make_shared< Container >();
	// We use the makeshared function instead of declaring new variables because they are automatically disposed
	auto form = std::make_shared<Form >(800, 100.0f, 480, 300, 2, 30, "FORM", true);
	{
		auto tabcontroller = std::make_shared<TabController>();
		form->Push(tabcontroller);
		//std::string name, float x, float y, float width, float height, int* selectedtab = nullptr
		auto tab = std::make_shared<Tab>("Tab",5,25,50,20,&SelectedTab);
		tabcontroller->Push(tab);
	}
	MenuEntity->Push(form);
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
