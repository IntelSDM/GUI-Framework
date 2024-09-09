#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "Button.h"
#include "Font.h";
#include "Graphics.h"

Button::Button(float x, float y, std::wstring text, std::function<void()> click)
{
	Button::Name = text;
	Button::Pos = {x, y};
	Button::Size = {70, 20};
	Button::Action = click;
	Button::SetWidth();
	SetVisible(true);
}

void Button::SetWidth()
{
	Button::Size.x = GetTextSize(Button::Name, "Verdana", 11).x + 20;
}

void Button::Update()
{
	if (!Button::Parent)
		Button::SetVisible(false);
	Button::ParentPos = Button::Parent->GetParentPos();
	if (!Button::IsVisible())
		return;


	if (!Button::Blocked)
	{
		if (IsMouseInRectangle(Button::Pos + ParentPos, Button::Size) && IsKeyClicked(VK_LBUTTON) && Button::LastClick < (clock() * 0.00001f))
		{
			Button::Action();
			Button::LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}
}

void Button::Draw()
{
	if (!Button::Parent)
		Button::SetVisible(false);
	if (!Button::IsVisible())
		return;
	
	MyColour rectColour = MenuColours["Button"];
	MyColour rectOutlineColour = MenuColours["ButtonOutline"];
	MyColour textColour = MenuColours["Text"];

	//if is hovering color
	if (IsMouseInRectangle(Button::Pos + ParentPos, Button::Size))
	{
		rectColour = MenuColours["ButtonHover"];
		if (IsKeyDown(VK_LBUTTON))
		{
			rectColour = MenuColours["ButtonActive"];
		}
	}

	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, rectColour);
	FilledRectangle(ParentPos.x + Button::Pos.x, ParentPos.y + Button::Pos.y, Button::Size.x, Button::Size.y, rectOutlineColour);
	DrawText(ParentPos.x + Button::Pos.x + (Button::Size.x / 2), ParentPos.y + Button::Pos.y + (Button::Size.y / 2), Button::Name, "Verdana", 12, textColour, CentreCentre);
	DrawTooltip();
}
