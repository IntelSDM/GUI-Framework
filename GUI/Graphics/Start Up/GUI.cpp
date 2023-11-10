#include "pch.h"
#include "GUI.h"
#include "entity.h"
#include "Form.h"
#include "Button.h"
#include "ColourPicker.h"
#include "Label.h"
#include "tab.h"
#include "TabController.h"
#include "Toggle.h"
#include "Slider.h"
#include "DropDown.h"
#include "ComboBox.h"
#include "KeyBind.h"
#include "TabListBox.h"
#include "TabListBoxController.h"
#include "TextBox.h"
int SelectedTab = 1;
int SelectedSubTab = 0;
int TabCount = 0;
int KeyBindClipBoard = 0;
EntityVector MenuEntity;
bool MenuOpen = true;
D2D1::ColorF ColourPickerClipBoard = D2D1::ColorF::Red;
D2D1::ColorF ColourPick = Colour(0, 150, 255, 255);
bool ToggleTest = true;
int SliderInt = 50;
float SliderFloat = 50.0f;
int DropDownValue = 0;
bool Combo1 = true;
bool Combo2 = false;
bool Combo3 = false;
bool Combo4 = true;
bool Combo5 = false;
int Key = 0;
std::wstring TextBoxText = L"Text Box";
void CreateGUI()
{
	MenuEntity = std::make_shared< Container >();
	auto form = std::make_shared<Form >(100, 100.0f, 533, 350, 2, 30, L"FORM", false);
	{
		auto tabcontroller = std::make_shared<TabController>();
		form->Push(tabcontroller);

		auto tab = std::make_shared<Tab>(L"Tab1", 5, 55, 50, 20, &SelectedTab);
		{
			auto toggle = std::make_shared<Toggle>(10, 10, L"Toggle", &ToggleTest);
			tab->Push(toggle);
			auto button = std::make_shared<Button>(10, 30, L"Buttons", []() {Beep(100, 10); });
			tab->Push(button);
			auto colourpicker = std::make_shared<ColourPicker>(75, 10, &ColourPick);
			tab->Push(colourpicker);
			auto label = std::make_shared<Label>(L"Label", 10, 55);
			tab->Push(label);
			auto sliderint = std::make_shared<Slider<int>>(10, 80, L"Slider Int", L"%", 0, 100, &SliderInt);
			tab->Push(sliderint);
			auto sliderfloat = std::make_shared<Slider<float>>(10, 110, L"Slider Float", L"%", 0.0f, 100.0f, &SliderFloat);
			tab->Push(sliderfloat);
			std::list<std::wstring> downvalues = { L"Value 12345", L"Value 2", L"Value 3", L"Value 4" , L"Value 5", L"Value 6", L"Value 7" , L"Value 8" , L"Value 9" , L"Value 9" };
			auto dropdown = std::make_shared<DropDown>(10, 140, L"DropDown", &DropDownValue, downvalues);
			tab->Push(dropdown);
			std::list<std::wstring> combovalues = { L"Value 12345", L"Value 2", L"Value 3", L"Value 4" , L"Value 5", L"Value 6", L"Value 7" , L"Value 8" };
			std::list<bool*> bools = { &Combo1 ,&Combo2 ,&Combo3 ,&Combo4 ,&Combo5,&Combo5 ,&Combo5 ,&Combo5 };
			auto combo = std::make_shared<ComboBox>(10, 170, L"ComboBox", bools, combovalues);
			tab->Push(combo);
			auto keybind = std::make_shared<KeyBind>(10, 215, L"KeyBind", &Key);
			tab->Push(keybind);
			auto textbox = std::make_shared<TextBox>(10, 255, L"Textbox", &TextBoxText);
			tab->Push(textbox);
		}
		auto tab1 = std::make_shared<Tab>(L"Tab2", 65, 55, 50, 20, &SelectedTab);
		{
			auto tablist = std::make_shared<TabListBoxController>(10, 40, 160, 160);
			auto listtab1 = std::make_shared<TabListBox>(L"List Tab 1");
			{
				auto label = std::make_shared<Label>(L"List Instance 1", 180, 10);
				listtab1->Push(label);
			}
			auto listtab2 = std::make_shared<TabListBox>(L"List Tab 2");
			{
				auto label = std::make_shared<Label>(L"List Instance 2", 180, 10);
				listtab2->Push(label);
			}
			auto listtab3 = std::make_shared<TabListBox>(L"List Tab 3");
			{
				auto label = std::make_shared<Label>(L"List Instance 3", 180, 10);
				listtab3->Push(label);
			}
			auto listtab4 = std::make_shared<TabListBox>(L"List Tab 4");
			{
				auto label = std::make_shared<Label>(L"List Instance 4", 180, 10);
				listtab4->Push(label);
			}
			auto listtab5 = std::make_shared<TabListBox>(L"List Tab 5");
			{
				auto label = std::make_shared<Label>(L"List Instance 5", 180, 10);
				listtab5->Push(label);
			}
			auto listtab6 = std::make_shared<TabListBox>(L"List Tab 6");
			{
				auto label = std::make_shared<Label>(L"List Instance 6", 180, 10);
				listtab6->Push(label);
			}
			tablist->PushBack(listtab1);
			tablist->PushBack(listtab2);
			tablist->PushBack(listtab3);
			tablist->PushBack(listtab4);
			tablist->PushBack(listtab5);
			tablist->PushBack(listtab6);
			tab1->Push(tablist);
		}
		auto tab2 = std::make_shared<Tab>(L"Tab3", 125, 55, 50, 20, &SelectedTab);
		{
		}
tabcontroller->Push(tab);
tabcontroller->Push(tab1);
tabcontroller->Push(tab2);
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
