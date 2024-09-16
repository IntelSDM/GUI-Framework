#include "pch.h"
#include "Radar.h"
#include "Drawing.h"
#include "GUI.h"
#include "Animation.h"
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
	Radar::RealRadarSize = { bitmap->GetSize().width - 1000, bitmap->GetSize().height - 1000 };
	Radar::MapOffset = { bitmap->GetSize().width/2, bitmap->GetSize().height / 2 };
	
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
		if (tempvaluex > Bitmap->GetSize().width)
			tempvaluex = Bitmap->GetSize().width;
		if (tempvaluey < 0)
			tempvaluey = 0;
		if (tempvaluey > Bitmap->GetSize().height)
			tempvaluey = Bitmap->GetSize().height;
		*Radar::X = tempvaluex;
		*Radar::Y = tempvaluey;
		printf("!!!!!!!!!!X: %d Y: %d\n", *Radar::X, *Radar::Y);
	}
	if (IsMouseInRectangle(*Radar::X - Radar::Border.x, *Radar::Y - Radar::Border.y, *Radar::Width + (Radar::Border.x * 2), Radar::Border.y))
	{
		if (IsKeyClicked(VK_LBUTTON)) // This prevents a user holding down and hovering over the title bar to drag it. You need to actually click down.
			Radar::Dragging = true;

		Radar::Drag.x = Radar::Cursor.x - *Radar::X;
		Radar::Drag.y = Radar::Cursor.y - *Radar::Y;
	}

}
void Radar::AddPointOfInterest(Vector3 WorldPos)
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

	if (PointsOfInterest.empty())
		return;
	Vector3 WorldPos = PointsOfInterest[0];
	Vector2 pos = Vector2(*Radar::X, *Radar::Y);

	Vector2 targetScreenPos = Vector2(
		pos.x + (WorldPos.x * Radar::Scale * Zoom) + (MapOffset.x * Zoom) + MapOffset.x,
		pos.y - (WorldPos.z * Radar::Scale * Zoom) + (MapOffset.y * Zoom) + MapOffset.y
	);

	Vector2 screenCenter = Vector2(pos.x + *Radar::Width / 2, pos.y + *Radar::Height / 2);
	Vector2 diff = screenCenter - targetScreenPos;
	MapOffset = VecLerp(MapOffset, MapOffset + diff, 0.1);
	if (std::abs(diff.x) < 1 && std::abs(diff.y) < 1)
		PointsOfInterest.erase(PointsOfInterest.begin());

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

	Vector2 start = Vector2(MapOffset.x - ((*Radar::Width / 2) * Zoom), MapOffset.y - ((*Radar::Height / 2) * Zoom));
	Vector2 end = Vector2(MapOffset.x + ((*Radar::Width / 2) * Zoom), MapOffset.y + ((*Radar::Height / 2) * Zoom));
	float mapWidth = end.x - start.x;
	float mapHeight = end.y - start.y;

	// Map the world coordinates to a position on the radar
	float radarX = ((*Radar::X) + ((worldPos.x - start.x) / mapWidth) * (*Radar::Width));
	float radarY = ((*Radar::Y) + ((worldPos.z - start.y) / mapHeight) * (*Radar::Height));  // Use worldPos.z for Y mapping
	return Vector2(radarX, radarY);
	return end;
	return Vector2(worldPos.x, worldPos.z);
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
	if (CentreRadar)
	{
	//	AddPointOfInterest(Vector3(-500, 500, -1500));
	}
	Vector2 pos1 = WorldToRadar(Vector3(849, 0, 811)); // should be top right
//	Vector2 pos2 = WorldToRadar(Vector3(1385, 0, 142)); // should be top right
	Vector2 pos2 = WorldToRadar(Vector3(2000, 0, 2000));
	printf("X: %f Y: %f\n", pos1.x, pos1.y);
	printf("X: %f Y: %f\n", pos2.x, pos2.y);
	printf("X: %f Y: %f\n", MapOffset.x, MapOffset.y);
	FilledRectangle(pos1.x, pos1.y, 5, 5, MyColour(255, 0, 0, 255));
	FilledRectangle(pos2.x, pos2.y, 5, 5, MyColour(255, 255, 255, 255));
	//FilledRectangle(MapOffset.x, MapOffset.y, 5, 5, MyColour(255, 255, 255, 255));
}