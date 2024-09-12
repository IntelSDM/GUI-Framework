#pragma once
#include "Entity.h"
#include "Drawing.h"
#include "Input.h"

// due to using templates in c++ classes it is illegal to use a c++ file so this will look rather odd to the other code.
template <typename T>
class Slider : public Entity
{
protected:
	T MinValue;
	T MaxValue;
	T* Value;
	T OutputValue;

	bool Held = false;

	std::wstring Measurement;
	std::wstring OutputString;

	int TextSize = 11;
	std::string Font = "Verdana";

	void ConvertValueToString()
	{
		OutputString = std::to_wstring(OutputValue);
		size_t dotpos = OutputString.find(L".");
		if (dotpos != std::string::npos)
		{
			if (OutputString.length() > dotpos + 2)
			{
				OutputString.resize(dotpos + 3); // Truncate to two places after the decimal place
			}
		}
	}

public:
	void Update()
	{
		if (!Parent)
			SetVisible(false);
		ParentPos = Parent->GetParentPos();
		if (!IsVisible())
			return;

	

		if (!IsKeyDown(VK_LBUTTON))
		{
			if (Held == true)
				ValueChangeEvent();
			Held = false;
		}
		if (IsMouseInRectangle(ParentPos.x + Pos.x - 2, ParentPos.y + (Pos.y + 15) - 2, Size.x + 3, Size.y + 3) && IsKeyClicked(VK_LBUTTON) && !Blocked)
		{
			Held = true;
		}
		if (Held)
		{
			// make sure no clicks go through when holding.
			const float clamp = std::clamp<float>((float)MousePos.x - (float)(Pos.x + ParentPos.x), 0.00f, (float)Size.x);
			const float ratio = clamp / Size.x;
			*Value = MinValue + (MaxValue - MinValue) * ratio;
			OutputValue = *Value;
			ConvertValueToString();
		}
	}

	void Draw()
	{
		if (!IsVisible())
			return;
		if (!Parent)
			SetVisible(false);

		MyColour rectColour = MenuColours["Slider"];
		MyColour textColour = MenuColours["Text"];
		MyColour insideColour = MenuColours["SliderInside"];

		DrawText(ParentPos.x + Pos.x, (ParentPos.y + Pos.y) - 5, Name + L": " + OutputString + Measurement, Font, TextSize, textColour, None);
		OutlineRectangle(ParentPos.x + Pos.x - 1, ParentPos.y + (Pos.y + 15) - 1, Size.x + 2, Size.y + 2, 1, textColour);
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + (Pos.y + 15), Size.x, Size.y, rectColour);
		float ratio = (float)(*Value - (float)MinValue) / float(MaxValue - MinValue);
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + (Pos.y + 15), (int)Size.x * ratio, Size.y, insideColour);
		DrawTooltip();
		DrawTooltip(Vector2(Pos.x - 1, (Pos.y + 15)),Vector2(Size.x + 2, Size.y + 2));
	}

	Slider(int x, int y, std::wstring name, std::wstring measurement, T minvalue, T maxvalue, T* value) : Measurement(measurement), MaxValue(maxvalue), MinValue(minvalue), Value(value)
	{
		Pos = {(float)x, (float)y};
		Name = name;
		Size = {100, 7};
		OutputValue = *Value;
		ConvertValueToString();
		SetVisible(true);
	}
};
