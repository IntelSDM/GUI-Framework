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
#include "NumericSlider.h"
#include "LoadingBar.h"
#include "AssetManager.h"
#include "DropDownTextBox.h"
#include "Radar.h"
#include "LanguageSelector.h"
int SelectedTab = 1;
int SelectedSubTab = 0;
int TabCount = 0;
int KeyBindClipBoard = 0;
EntityVector MenuEntity;
bool MenuOpen = true;
D2D1::ColorF ColourPickerClipBoard = Colour(255,255,255);
D2D1::ColorF ColourPick = Colour(255, 0, 0, 255);
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
std::wstring PasswordText = L"Password";

int Radarx = 0;
int Radary = 50;
int RadarWidth = 500;
int RadarHeight = 500;
float RadarSizeX = 4000;
float RadarSizeY = 4000;
Vector3 LocalPlayerPos(0, 0, 0);
bool RadarEnabled = true;
void CreateGUI()
{
	MenuEntity = std::make_shared<Container>();
	AddTranslation(Languages::Chinese, L"FORM", L"表格");
	auto form = std::make_shared<Form>(100, 100.0f, 533, 350, 2, 30, L"FORM", false, GetFormIconPath());
	{
	
		auto tabcontroller = std::make_shared<TabController>();
		form->Push(tabcontroller);

		
		AddTranslation(Languages::Chinese, L"Tab", L"选项卡");
		auto tab = std::make_shared<Tab>(L"Tab1", 5, 55, &SelectedTab,50,20);
		{
			
		
			AddTranslation(Languages::Chinese, L"Toggle", L"切换");
			auto toggle = std::make_shared<Toggle>(10, 10, L"Toggle", &ToggleTest);
			tab->Push(toggle);
			toggle->SetToolTip(L"Toggle Tooltip");
			AddTranslation(Languages::Chinese, L"Toggle Tooltip", L"切换工具提示");
			auto button = std::make_shared<Button>(10, 30, L"Buttons", []() { Beep(100, 10); });
			AddTranslation(Languages::Chinese, L"Buttons", L"按钮");
			tab->Push(button);
			button->SetToolTip(L"Button Tooltip");
			AddTranslation(Languages::Chinese, L"Button Tooltip", L"按钮工具提示");
			auto colourpicker = std::make_shared<ColourPicker>(75, 10, &ColourPick);
			tab->Push(colourpicker);
			auto langselector = std::make_shared<LanguageSelector>(150, 10);
			tab->Push(langselector);
			auto label = std::make_shared<Label>(L"Label", 10, 55);
			AddTranslation(Languages::Chinese, L"Label", L"标签");
			tab->Push(label);
			auto sliderint = std::make_shared<Slider<int>>(10, 80, L"Slider Int", L"%", 0, 100, &SliderInt);
			AddTranslation(Languages::Chinese, L"Slider Int", L"滑块整数");
			tab->Push(sliderint);
			auto sliderfloat = std::make_shared<Slider<float>>(10, 110, L"Slider Float", L"%", 0.0f, 100.0f, &SliderFloat);
			AddTranslation(Languages::Chinese, L"Slider Float", L"滑块浮点数");
			tab->Push(sliderfloat);
			std::vector<std::wstring> downvalues = {L"Value 12345", L"Value 2", L"Value 3", L"Value 4", L"Value 5", L"Value 6", L"Value 7", L"Value 8", L"Value 9", L"Value 9"};
			std::vector<std::wstring> chinesedownvalues = {L"值 12345", L"值 2", L"值 3", L"值 4", L"值 5", L"值 6", L"值 7", L"值 8", L"值 9", L"值 9"};
			AddTranslation(Languages::Chinese, downvalues, chinesedownvalues);

			auto dropdown = std::make_shared<DropDown>(10, 140, L"DropDown", &DropDownValue, downvalues);
			AddTranslation(Languages::Chinese, L"DropDown", L"下拉框");
			tab->Push(dropdown);
			std::vector<std::wstring> combovalues = {L"Value 12345", L"Value 2", L"Value 3", L"Value 4", L"Value 5", L"Value 6", L"Value 7", L"Value 8"};
			std::vector<bool*> bools = {&Combo1, &Combo2, &Combo3, &Combo4, &Combo5, &Combo5, &Combo5, &Combo5};
			auto combo = std::make_shared<ComboBox>(10, 170, L"ComboBox", bools, combovalues);
			AddTranslation(Languages::Chinese, L"ComboBox", L"组合框");
			tab->Push(combo);
			auto keybind = std::make_shared<KeyBind>(10, 215, L"KeyBind", &Key);
			AddTranslation(Languages::Chinese, L"KeyBind", L"键绑定");
			tab->Push(keybind);
			auto textbox = std::make_shared<TextBox>(10, 255, L"Textbox", &TextBoxText,false,false);
			AddTranslation(Languages::Chinese, L"Textbox", L"文本框");
			tab->Push(textbox);
			auto passwordbox = std::make_shared<TextBox>(250, 255, L"Password", &PasswordText, true);
			AddTranslation(Languages::Chinese, L"Password", L"密码");
			tab->Push(passwordbox);
			auto numericslider = std::make_shared<NumericSlider<float>>(250, 10, L"Numeric Slider", L"M", 30.0f, 10000.0f, &SliderFloat);
			tab->Push(numericslider);
		
		}
		auto tab1 = std::make_shared<Tab>(L"Tab2", 65, 55, &SelectedTab,50,20);
		{
			auto tablist = std::make_shared<TabListBoxController>(10, 40, 160, 160);
			AddTranslation(Languages::Chinese, L"TabListBoxController", L"选项卡列表控制器");
			AddTranslation(Languages::Chinese, L"List Tab 1", L"列表选项卡 1");
			auto listtab1 = std::make_shared<TabListBox>(L"List Tab 1");
			{
				auto label = std::make_shared<Label>(L"List Instance 1", 180, 10);
				listtab1->Push(label);
				std::vector<std::wstring> downvalues = { L"Value 12345678910", L"Value 2", L"Value 3", L"Value 4", L"Value 5", L"Value 6", L"Value 7", L"Value 8", L"Value 9", L"Value 9" };
				auto textbox1 = std::make_shared<DropDownTextBox>(180, 100, L"TextDropDown", &DropDownValue, downvalues);
				listtab1->Push(textbox1);

			}
			AddTranslation(Languages::Chinese, L"List Tab 2", L"列表选项卡 2");
			auto listtab2 = std::make_shared<TabListBox>(L"List Tab 2");
			{
				auto label = std::make_shared<Label>(L"List Instance 2", 180, 10);
				listtab2->Push(label);
			}
			AddTranslation(Languages::Chinese, L"List Tab 3", L"列表选项卡 3");
			auto listtab3 = std::make_shared<TabListBox>(L"List Tab 3");
			{
				auto label = std::make_shared<Label>(L"List Instance 3", 180, 10);
				listtab3->Push(label);
			}
			AddTranslation(Languages::Chinese, L"List Tab 4", L"列表选项卡 4");
			auto listtab4 = std::make_shared<TabListBox>(L"List Tab 4");
			{
				auto label = std::make_shared<Label>(L"List Instance 4", 180, 10);
				listtab4->Push(label);
			}
			AddTranslation(Languages::Chinese, L"List Tab 5", L"列表选项卡 5");
			auto listtab5 = std::make_shared<TabListBox>(L"List Tab 5");
			{
				auto label = std::make_shared<Label>(L"List Instance 5", 180, 10);
				listtab5->Push(label);
			}
			AddTranslation(Languages::Chinese, L"List Tab 6", L"列表选项卡 6");
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

		auto tab2 = std::make_shared<Tab>(L"Tab3", 125, 55, &SelectedTab, 50, 20);
		{
	
			AddTranslation(Languages::Chinese, L"Test", L"测试");
						auto loadingbar = std::make_shared<LoadingBar>(50, 50, 100, 100,L"Test",&SliderInt, GetFormIconPath());
			tab2->Push(loadingbar);
		}
		tabcontroller->Push(tab);
		tabcontroller->Push(tab1);
		tabcontroller->Push(tab2);
	}
	ID2D1Bitmap* Bitmap = nullptr;
	CreateDirectXBitmap(GetMapPath(), &Bitmap);
	auto radar = std::make_shared<Radar>(&Radarx, &Radary, &RadarWidth, &RadarHeight, &RadarEnabled,&LocalPlayerPos, Bitmap);


	MenuEntity->Push(form);
	MenuEntity->Push(radar);
	MenuEntity->Draw();
	MenuEntity->Update();
}

void SetFormPriority()
{
	// This sorts the host container (containerptr) which contains forms, as long as a form isn't parented to another form then this will allow it to draw over when clicked.
	// I swear to god if i need to make this work for forms inside forms for some odd reason in the future then i am going to throw a monitor out the window.
	std::sort(MenuEntity->GetContainer().begin(), MenuEntity->GetContainer().end(),
	          [](child a, child b) { return b->GetLastClick() < a->GetLastClick(); }
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
