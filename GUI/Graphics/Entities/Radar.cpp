#include "pch.h"
#include "Radar.h"
#include "Drawing.h"
#include "GUI.h"
#include "Animation.h"
// Radar maths is specifically for the game RUST, You must adapt the maths to your own needs. 
Radar::Radar(int* x, int* y, int* width, int* height, bool* enabled, Vector3* localplayerpos, ID2D1Bitmap* bitmap = nullptr)
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
	Radar::MapOffset = { bitmap->GetSize().width/2, bitmap->GetSize().height / 2 };
	Radar::LocalPlayerPos = localplayerpos;
}

Radar::Radar(int* x, int* y, int* width, int* height, bool* enabled,Vector3* localplayerpos, float* mapsizex, float* mapsizey)
{
	Radar::X = x;
	Radar::Y = y;
	Radar::Width = width;
	Radar::Height = height;
	Radar::Enabled = enabled;
	Radar::SetVisible(true);
	Radar::Border = { 4,20 };
	Radar::Dragging = false;
	Radar::Drag = { 0, 0 };
	Radar::MapSizeX = mapsizex;
	Radar::MapSizeY = mapsizey;
	Radar::MapOffset = {  *mapsizex / 2, *mapsizey / 2 };
	Radar::LocalPlayerPos = localplayerpos;
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
	if (Radar::Blocked)
		return;
	if (!MenuOpen)
		return;
	if (Radar::Dragging && !IsKeyDown(VK_LBUTTON))
		Radar::Dragging = false;
	if (Radar::Moving || Radar::Stretching)
		return;
	if (Radar::Dragging)
	{ 
		float tempvaluex  = Radar::Cursor.x - Radar::Drag.x;
		float tempvaluey = Radar::Cursor.y - Radar::Drag.y;
		if (tempvaluex < 0)
			tempvaluex = 0;
		if (tempvaluex > (Bitmap != nullptr ? Radar::Bitmap->GetSize().width  : *Radar::MapSizeX))
			tempvaluex = (Bitmap != nullptr ? Radar::Bitmap->GetSize().width : *Radar::MapSizeX);
		if (tempvaluey < 0)
			tempvaluey = 0;
		if (tempvaluey > (Bitmap != nullptr ? Radar::Bitmap->GetSize().width : *Radar::MapSizeY))
			tempvaluey = (Bitmap != nullptr ? Radar::Bitmap->GetSize().width : *Radar::MapSizeY);
		*Radar::X = tempvaluex;
		*Radar::Y = tempvaluey;
	}
	if (IsMouseInRectangle(*Radar::X - Radar::Border.x, *Radar::Y - Radar::Border.y, *Radar::Width + (Radar::Border.x * 2), Radar::Border.y))
	{
		if (IsKeyClicked(VK_LBUTTON)) // This prevents a user holding down and hovering over the title bar to drag it. You need to actually click down.
			Radar::Dragging = true;

		Radar::Drag.x = Radar::Cursor.x - *Radar::X;
		Radar::Drag.y = Radar::Cursor.y - *Radar::Y;
	}

}
void Radar::AddPointOfInterest(Vector2 WorldPos)
{
	Radar::PointsOfInterest.push_back(WorldPos);
}
void Radar::StretchAction()
{
	if (Radar::Blocked)
		return;
	if (!MenuOpen)
		return;
	if (Radar::Moving || Radar::Dragging)
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
void Radar::HandlePointsOfInterest()
{
	if (CentreRadar)
	{
		MapOffset.x = LocalPlayerPos->x + (Bitmap != nullptr ? Radar::Bitmap->GetSize().width / 2 : *Radar::MapSizeX / 2);
		MapOffset.y = (Bitmap != nullptr ? Radar::Bitmap->GetSize().height / 2 : *Radar::MapSizeY / 2) - LocalPlayerPos->z;
	}

	for (const auto& point : PointsOfInterest)
	{
		Vector2 radarPos = WorldToRadar(Vector3(point.x, 0, point.y));
		FilledRectangle(radarPos.x, radarPos.y, 5, 5, Colour(0, 255, 0));  
	}
}
void Radar::MapInput()
{
	if (Radar::Blocked)
		return;
	if (Radar::Dragging || Radar::Stretching)
		return;
	if (Radar::Moving && !IsKeyDown(VK_LBUTTON))
		Radar::Moving = false;

	Vector2 point1 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height);
	Vector2 point2 = Vector2(*Radar::X + *Radar::Width - 10, *Radar::Y + *Radar::Height);
	Vector2 point3 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height - 10);
	if (IsMouseInTriangle(point1, point2, point3)) // Stops it messing with the stretching functionality
		return;

	if (Radar::Moving)
	{
		// This is inverted so it moves in the right direction that you move against it
		MapOffset.x = Radar::Movement.x - Radar::Cursor.x; 
		MapOffset.y = Radar::Movement.y - Radar::Cursor.y; 
	}

	if (IsMouseInRectangle(*Radar::X, *Radar::Y, *Radar::Width, *Radar::Height))
	{
		if (IsKeyClicked(VK_LBUTTON))
		{
			Radar::Moving = true;
			CentreRadar = false;
			Radar::Movement.x = Radar::Cursor.x + MapOffset.x;  
			Radar::Movement.y = Radar::Cursor.y + MapOffset.y;
		}
	}
}
void Radar::Zooming()
{
	if (Radar::Blocked)
		return;
	if (Radar::Moving || Radar::Dragging || Radar::Stretching)
		return;
	if (IsMouseInRectangle(*Radar::X, *Radar::Y, *Radar::Width, *Radar::Height))
	{
		if (HasScrolledUp())
		{
			if (Radar::Zoom > 0.1f)
			{
				Radar::Zoom -= 0.15f;
			}
		}
		if (HasScrolledDown())
		{
			

			if (Radar::Zoom < 2.5f)
			{
				Radar::Zoom += 0.15f;
			}

		}
	}
}
Vector2 Radar::WorldToRadar(Vector3 worldPos)
{

	worldPos.x += (Bitmap != nullptr ? Radar::Bitmap->GetSize().width /2  : *Radar::MapSizeX/2);
	worldPos.z = (Bitmap != nullptr ? Radar::Bitmap->GetSize().height /2 : *Radar::MapSizeY/2) - worldPos.z;


	Vector2 start = Vector2(MapOffset.x - ((*Radar::Width / 2) * Zoom), MapOffset.y - ((*Radar::Height / 2) * Zoom));
	Vector2 end = Vector2(MapOffset.x + ((*Radar::Width / 2) * Zoom), MapOffset.y + ((*Radar::Height / 2) * Zoom));
	float mapWidth = end.x - start.x;
	float mapHeight = end.y - start.y;

	float radarX = ((*Radar::X) + ((worldPos.x - start.x) / mapWidth) * (*Radar::Width));
	float radarY = ((*Radar::Y) + ((worldPos.z - start.y) / mapHeight) * (*Radar::Height));
	
	return Vector2(radarX, radarY);
}

void Radar::Draw()
{
	if (!Radar::Parent)
		Radar::SetVisible(false);
	if (!Radar::IsVisible())
		return;
	if (!*Radar::Enabled)
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
		FilledRectangle(*Radar::X - 1, *Radar::Y - 1, *Radar::Width + 2, *Radar::Height + 2, rectOutlineColour);
	}
	FilledRectangle(*Radar::X, ParentPos.y + *Radar::Y, *Radar::Width, *Radar::Height, rectBackground);
	if(Radar::Bitmap)
	DrawBitmap(Bitmap, *Radar::X, *Radar::Y, *Radar::Width, *Radar::Height, MapOffset.x - ((*Radar::Width / 2) * Zoom), MapOffset.y - ((*Radar::Height / 2) * Zoom), MapOffset.x + ((*Radar::Width / 2) * Zoom), MapOffset.y + ((*Radar::Height / 2) * Zoom));
	HandlePointsOfInterest();
	MapInput();
	Zooming();
	if (MenuOpen)
	{
		Vector2 point1 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height);
		Vector2 point2 = Vector2(*Radar::X + *Radar::Width - 10, *Radar::Y + *Radar::Height);
		Vector2 point3 = Vector2(*Radar::X + *Radar::Width, *Radar::Y + *Radar::Height - 10);
		FilledTriangle(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, rectOutlineColour);

	}

	MyColour iconcolour1 = MenuColours["Text"];
	MyColour iconcolour2 = MenuColours["Text"];
	MyColour iconcolour3 = MenuColours["Text"];
	if (IsMouseInRectangle(*Radar::X, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17))
	{
		iconcolour1 = MenuColours["Radar"];
		if (IsKeyClicked(VK_LBUTTON))
		{
			CentreRadar = true;
			LastWidgetClick = GetTickCount64() + 35;
		}
	
	}
	if (IsMouseInRectangle(*Radar::X + 18, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17))
	{
		iconcolour2 = MenuColours["Radar"];
		if (IsKeyClicked(VK_LBUTTON) && LastWidgetClick < GetTickCount64())
		{
			if (Radar::Zoom > 0.1f)
			{
				Radar::Zoom -= 0.10f;
			}
				LastWidgetClick = GetTickCount64() + 45;
		}

	}
	if (IsMouseInRectangle(*Radar::X + 36, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17))
	{
		iconcolour3 = MenuColours["Radar"];
		if (IsKeyClicked(VK_LBUTTON) && LastWidgetClick < GetTickCount64())
		{
			if (Radar::Zoom < 2.5f)
			{
				Radar::Zoom += 0.10f;
			}
			LastWidgetClick = GetTickCount64() + 45;
		}

	}
	OutlineRectangle(*Radar::X, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17, 1, Colour(255,255,255));
	FilledRectangle(*Radar::X, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 16, 16, rectOutlineColour);
	DrawText(*Radar::X + 9, *Radar::Y + *Radar::Height + Radar::Border.x + 8, L"🏠︎", Font, TextSize, iconcolour1, CentreCentre);

	OutlineRectangle(*Radar::X + 18, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17, 1, Colour(255, 255, 255));
	FilledRectangle(*Radar::X + 18, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 16, 16, rectOutlineColour);
	DrawText(*Radar::X + 27, *Radar::Y + *Radar::Height + Radar::Border.x + 8, L"+", Font, TextSize, iconcolour2, CentreCentre);

	OutlineRectangle(*Radar::X + 36, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 17, 17, 1, Colour(255, 255, 255));
	FilledRectangle(*Radar::X + 36, *Radar::Y + *Radar::Height + Radar::Border.x + 1, 16, 16, rectOutlineColour);
	DrawText(*Radar::X + 44, *Radar::Y + *Radar::Height + Radar::Border.x + 8, L"-", Font, TextSize, iconcolour3, CentreCentre);

	Vector2 pos = WorldToRadar(*LocalPlayerPos);
	FilledRectangle(pos.x, pos.y, 10,10, Colour(255,0,0));
}