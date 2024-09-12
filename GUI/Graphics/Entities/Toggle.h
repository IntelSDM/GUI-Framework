#pragma once
#include "Entity.h"

class Toggle : public Entity
{
protected:
	bool* Data;
	bool Tabbed = false;

	Vector2 TextMeasurement = Vector2(0, 0);
	int TextSize = 12;
	std::string Font = "Verdana";
public:
	Toggle(float x, float y, std::wstring text, bool* data);
	void Update();
	void Draw();
};
