#include "pch.h"
#include "Toggle.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"

Toggle::Toggle(float x, float y, std::wstring text, bool* data = nullptr)
{
	Toggle::Pos = {x, y};
	Toggle::Size = {11, 11};
	Toggle::Name = text;
	Toggle::TextSize = GetTextSize(text, "Verdana", 12);
	Data = data;
}

void Toggle::Update()
{
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	Toggle::ParentPos = Toggle::Parent->GetParentPos();
	if (!Toggle::Blocked)
	{
		// square click check
		if (IsMouseInRectangle(Toggle::Pos + Toggle::ParentPos, Toggle::Size) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{
			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();
		}
		// text click check
		Vector2 textdimensions = {Toggle::TextSize.x + 4, Toggle::TextSize.y};
		if (IsMouseInRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x), Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextSize.x, Toggle::TextSize.y) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClick < (clock() * 0.00001f))
		{
			*Toggle::Data = !(*Toggle::Data);
			Toggle::LastClick = (clock() * 0.00001f) + 0.002f;
			Toggle::ValueChangeEvent();
		}
	}
}

void Toggle::Draw()
{
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, Colour(130, 130, 130, 255));
	FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, Colour(80, 80, 80, 255));
	if (*Toggle::Data == true)
		FilledRectangle(ParentPos.x + Pos.x + 1, ParentPos.y + Pos.y + 1, Size.x - 2, Size.y - 2, Colour(255, 0, 0, 255));

	DrawText(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 3, Toggle::ParentPos.y + Toggle::Pos.y - (Toggle::Size.y / 8), Toggle::Name, "Verdana", 12, Colour(255, 255, 255, 255), None);
}
