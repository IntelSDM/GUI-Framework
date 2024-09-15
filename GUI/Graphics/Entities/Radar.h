#pragma once
#include "Entity.h"

class Radar : public Entity
{
protected:
	int* X = nullptr;
	int* Y = nullptr;
	int* Width = nullptr;
	int* Height = nullptr;
	bool* Enabled = nullptr;
	ID2D1Bitmap* Bitmap = nullptr;
	int TextSize = 13;
	std::string Font = "Verdana";

	Vector2 Drag;
	Vector2 Cursor;
	Vector2 Border;
	bool Dragging = false;
	void DragAction();
	void StretchAction();
	void MapInput();
	void AddPointOfInterest(Vector3 pos);
	void HandlePointsOfInterest();
	bool Stretching = false;
	Vector2 Stretch;
	float Zoom = 2.0f;
	float Scale = 1.0f;
	Vector2 MapOffset;
	std::vector<Vector3> PointsOfInterest;
public:
	Radar(int* x, int* y, int* width, int* height,bool* enabled, ID2D1Bitmap* bitmap);
	void Update();
	void Draw();
};
