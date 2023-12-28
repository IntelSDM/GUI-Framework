#include "pch.h"
#include "Tab.h"
#include "Input.h"
#include "Gui.h"
#include "Drawing.h"

Tab::Tab(std::wstring name, float x, float y, int* selectedtab, float width, float height)
{
	//Calc Size based off the name size
	Vector2 txtSize = GetTextSize(name, "Verdana", 12);
	if (width == 0 && height == 0)
		Tab::Size = txtSize;
	else
	{
		//Let's make it impossible to make it smaller than the actual font size.. otherwise it looks so weird lol
		if (width < txtSize.x)
			width = txtSize.x;
		if (height < txtSize.y)
			height = txtSize.y;
		Tab::Size = {width, height};
	}

	Tab::SetVisible(true);
	Tab::Index = TabCount; // this doesn't need to be changed for a different set of tabs or whatever, you dont need to import a value here you just need a nonce value so you never get a repeated one
	Tab::Pos = {x, y};
	Tab::Selected = selectedtab;
	Tab::Name = name;
	TabCount++;
	SetVisible(true);
}

void Tab::Update()
{
	if (!Tab::Parent)
		Tab::SetVisible(false);

	if (!Tab::IsVisible())
		return;
	Tab::ParentPos = Tab::GetParent()->GetParentPos();
	Tab::ParentSize = Tab::GetParent()->GetSize();

	size_t ignorecount = 0;

	for (child& it : Tab::Parent->GetContainer())
	{
		if (it == shared_from_this())
			break;

		if (!it->IsVisible())
			++ignorecount;
	}
	bool selected = Index == *Selected;
	if (!selected && !Tab::Blocked)
	{
		if (IsMouseInRectangle(Tab::ParentPos.x + Tab::Pos.x, (Tab::ParentPos.y + Tab::ParentSize.y) - Tab::Pos.y, Tab::Size.x, Tab::Size.y) && IsKeyClicked(VK_LBUTTON) && !selected && Tab::LastClick < (clock() * 0.00001f))
		{
			*Selected = Index;
			// ideally you just want to make a timer and use that but since this is just poc we do this
			Tab::LastClick = (clock() * 0.00001f) + 0.002f;
			Tab::ValueChangeEvent();
		}
	}
}

void Tab::Draw()
{
	if (!Tab::IsVisible())
		return;

	MyColour rectColour = MenuColours["Tab"];
	MyColour textColour = MenuColours["Text"];
	bool selected = Index == *Selected;

	//if is hovering color
	if (IsMouseInRectangle(Tab::ParentPos.x + Tab::Pos.x, (Tab::ParentPos.y + Tab::ParentSize.y) - Tab::Pos.y, Tab::Size.x, Tab::Size.y))
	{
		rectColour = MenuColours["TabHover"];
		if (IsKeyDown(VK_LBUTTON))
		{
			rectColour = MenuColours["TabActive"];
		}
	}

	//If is selected
	if (selected)
		rectColour = MenuColours["TabActive"];

	FilledRectangle(Tab::ParentPos.x + Tab::Pos.x, (Tab::ParentPos.y + Tab::ParentSize.y) - Tab::Pos.y, Tab::Size.x, Tab::Size.y, rectColour);
	DrawText(Tab::ParentPos.x + Tab::Pos.x + (Tab::Size.x / 2), ((Tab::ParentPos.y + Tab::ParentSize.y) - Tab::Pos.y) + (Tab::Size.y / 2), Tab::Name, "Verdana", 12, textColour, CentreCentre);

	if (selected)
	{
		Container::Draw();
		Container::Update();
	}
}
