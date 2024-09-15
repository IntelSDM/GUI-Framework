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
public:
	Radar(int* x, int* y, int* width, int* height,bool* enabled, ID2D1Bitmap* bitmap);
	void Update();
	void Draw();
};
