#include "pch.h"
#include "GUI.h"
#include "entity.h"
#include "Form.h"
#include "Button.h"
EntityVector MenuEntity;
bool MenuOpen = true;

void CreateGUI()
{
	MenuEntity = std::make_shared< Container >();
	auto form = std::make_shared<Form >(100, 100.0f, 533, 350, 2, 30, L"FORM", false);
	{
		auto button = std::make_shared<Button>(100, 100, L"Buttons", []() {Beep(100, 10); });
		form->Push(button);
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
float LastOpen = 0;
void Render()
{
	if (IsKeyClicked(VK_INSERT) && LastOpen < clock() * 0.00001f)
	{
		LastOpen = (clock() * 0.00001f) + 0.002f;
		MenuOpen = !MenuOpen;
	}

	MenuEntity->Draw();
	MenuEntity->GetContainer()[0]->Update(); // only allow stretching,dragging and other update stuff if it is the main form, prevents dragging and sizing the wrong forms.
	SetFormPriority();
}
