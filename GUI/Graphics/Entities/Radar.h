#pragma once
#include "Entity.h"

class Radar : public Entity
{
protected:
	int* X = nullptr;
	int* Y = nullptr;
	int* Width = nullptr;
	int* Height = nullptr;
	float* MapSizeX = nullptr;
	float* MapSizeY = nullptr;
	bool* Enabled = nullptr;
	ID2D1Bitmap* Bitmap = nullptr;

	float Scale = 1.0f;
	float Zoom = 1.0f;
	
	int TextSize = 13;
	std::string Font = "Verdana";

	Vector2 Drag;
	Vector2 Cursor;
	Vector2 Border;
	Vector2 Stretch;
	Vector2 Movement;
	Vector2 MapOffset;

	bool Dragging = false;
	bool Moving = false;
	bool Stretching = false;
	bool CentreRadar = true;

	void DragAction();
	void StretchAction();
	void MapInput();
	void AddPointOfInterest(Vector2 pos);
	void HandlePointsOfInterest();
	void Zooming();
	
	Vector2 WorldToRadar(Vector3 pos);
	
	std::vector<Vector2> PointsOfInterest;

	Vector3* LocalPlayerPos = nullptr;
public:
	Radar(int* x, int* y, int* width, int* height,bool* enabled, Vector3* localplayerpos, ID2D1Bitmap* bitmap);
	Radar(int* x, int* y, int* width, int* height, bool* enabled, Vector3* localplayerpos, float* mapsizex, float* mapsizey);
	void Update();
	void Draw();
};
