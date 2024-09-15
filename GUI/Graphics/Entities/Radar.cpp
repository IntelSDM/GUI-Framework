#include "pch.h"
#include "Radar.h"
#include "Drawing.h"
#include "GUI.h"
Radar::Radar(int* x, int* y, int* width, int* height, bool* enabled, ID2D1Bitmap* bitmap = nullptr)
{
	Radar::X = x;
	Radar::Y = y;
	Radar::Width = width;
	Radar::Height = height;
	Radar::Enabled = enabled;
	Radar::Bitmap = bitmap;
	Radar::SetVisible(true);
	Radar::Border = { 4,20 };
	Radar::Dragging = false;
	Radar::Drag = { 0, 0 };
}

void Radar::Update()
{
	if (!Radar::Parent)
		Radar::SetVisible(false);
	Radar::ParentPos = Radar::Parent->GetParentPos();
	if (!Radar::IsVisible())
		return;
	if (!*Radar::Enabled)
		return;
	Radar::Cursor = MousePos;
	DragAction();
	if (MenuOpen)
	{
		StretchAction();
	}
}

void Radar::DragAction()
{
	if (Radar::Dragging && !IsKeyDown(VK_LBUTTON))
		Radar::Dragging = false;

	if (Radar::Dragging)
	{ 
		*Radar::X = Radar::Cursor.x - Radar::Drag.x;
		*Radar::Y = Radar::Cursor.y - Radar::Drag.y;
	}
	if (IsMouseInRectangle(*Radar::X - Radar::Border.x, *Radar::Y - Radar::Border.y, *Radar::Width + (Radar::Border.x * 2), Radar::Border.y))
	{
		if (IsKeyClicked(VK_LBUTTON)) // This prevents a user holding down and hovering over the title bar to drag it. You need to actually click down.
			Radar::Dragging = true;

		Radar::Drag.x = Radar::Cursor.x - *Radar::X;
		Radar::Drag.y = Radar::Cursor.y - *Radar::Y;
	}

}

void Radar::StretchAction()
{
	if (Radar::Blocked)
		return;
	if (!MenuOpen)
		return;
	Vector2 point1 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height);
	Vector2 point2 = Vector2(*Radar::X + *Radar::Width - 10, *Radar::Y + *Radar::Height);
	Vector2 point3 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height - 10);

	if (Radar::Stretching && !IsKeyDown(VK_LBUTTON))
	{
		Radar::Stretching = false;
	}

	if (Radar::Stretching)
	{
		float stretchx = Radar::Cursor.x - Radar::Stretch.x;
		float stretchy = Radar::Cursor.y - Radar::Stretch.y;
		
		Vector2 Stretched{ stretchx, stretchy };
		// Prevent the gui going below the original scale
		if (stretchx > 50 && stretchy >50)
		{
			*Radar::Width = stretchx;
			*Radar::Height = stretchy;
		}
	}

	if (IsMouseInTriangle(point1, point2, point3))
	{
		if (IsKeyClicked(VK_LBUTTON))
			Radar::Stretching = true;

		Radar::Stretch.x = Radar::Cursor.x - (*Radar::Width);
		Radar::Stretch.y = Radar::Cursor.y - (*Radar::Height);
	}
}
void Radar::Draw()
{
		if (!Radar::Parent)
		Radar::SetVisible(false);
	if (!Radar::IsVisible())
		return;
	if(!*Radar::Enabled)
		return;
	MyColour rectColour = MenuColours["Radar"];
	MyColour rectBackground = MenuColours["RadarBackground"];
	MyColour rectOutlineColour = MenuColours["RadarAccents"];
	MyColour textColour = MenuColours["Text"];

	if (IsMouseInRectangle(*Radar::X - Radar::Border.x - 1, *Radar::Y - Radar::Border.y - 1, *Radar::Width + (Radar::Border.x * 2) + 2, Radar::Border.y + *Radar::Height + Radar::Border.x + 2) && IsKeyClicked(VK_LBUTTON))
	{
		Radar::SetLastClick(clock() * 0.00001f); // we multiple it for accuracy purposes to turn it into a float
	}
	if (MenuOpen)
	{
		FilledRoundedRectangle(*Radar::X - Radar::Border.x - 1, *Radar::Y - Radar::Border.y - 1, *Radar::Width + (Radar::Border.x * 2) + 2, Radar::Border.y + *Radar::Height + Radar::Border.x + 2, 3, rectBackground);
		FilledRoundedRectangle(*Radar::X - Radar::Border.x, *Radar::Y - Radar::Border.y, *Radar::Width + (Radar::Border.x * 2), Radar::Border.y + *Radar::Height + Radar::Border.x, 3, rectColour);
		DrawText(*Radar::X + (Radar::Border.x * 2), *Radar::Y - (Radar::Border.y / 1.2), L"Radar", Font, TextSize, textColour, None);

	}
	else
	{
		FilledRectangle(*Radar::X  - 1, *Radar::Y - 1, *Radar::Width + 2, *Radar::Height + 2, rectOutlineColour);
	}
	FilledRectangle(*Radar::X, ParentPos.y + *Radar::Y, *Radar::Width, *Radar::Height, rectBackground);

	if (MenuOpen)
	{
		Vector2 point1 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height);
		Vector2 point2 = Vector2(*Radar::X + *Radar::Width - 10, *Radar::Y + *Radar::Height);
		Vector2 point3 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height - 10);
		FilledTriangle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, rectOutlineColour);

	}

}