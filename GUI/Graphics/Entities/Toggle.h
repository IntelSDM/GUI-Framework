#pragma once
#include "Entity.h"

class Toggle : public Entity
{
protected:
	bool* Data;
	bool Tabbed = false;

	Vector2 TextSize = Vector2(0, 0);

public:
	Toggle(float x, float y, std::wstring text, bool* data);
	void Update();
	void Draw();
};
