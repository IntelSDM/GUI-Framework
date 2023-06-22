#include "pch.h"
#include "Toggle.h"
#include "Input.h"
#include "Drawing.h"


Toggle::Toggle(float x, float y, std::string text, bool* data = nullptr)
{
	Toggle::Pos = { x,y };
	Toggle::Size = { 11,11 };
	Toggle::Name = text;
	GetTextSize(text, 12, &TextWidth, &TextHeight, "Verdana");

	Data = data;


}


void Toggle::Update()
{

	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	Toggle::ParentPos = Toggle::Parent->GetParent()->GetPos();
	 
	if (IsMouseInRectangle(Toggle::Pos + Toggle::ParentPos, Toggle::Size) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClickTime < (clock() * 0.00001f))
	{

		*Toggle::Data = !(*Toggle::Data);
		Toggle::LastClickTime = (clock() * 0.00001f) + 0.002f;
	}
	
	Vector2 textdimensions = { Toggle::TextWidth + 4,Toggle::TextHeight };
	if (IsMouseInRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 2, Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextWidth + 4, Toggle::TextHeight) && IsKeyClicked(VK_LBUTTON) && Toggle::LastClickTime < (clock() * 0.00001f))
	{

		*Toggle::Data = !(*Toggle::Data);
		Toggle::LastClickTime = (clock() * 0.00001f) + 0.002f;
	}

}

void Toggle::Draw()
{
	//Button::Parent->GetParent()->GetParent()->GetPos();
	if (!Toggle::Parent)
		Toggle::SetVisible(false);
	if (!Toggle::IsVisible())
		return;

	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, Colour(130, 130, 130, 255));
	FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, Colour(80, 80, 80, 255));
	if (*Toggle::Data == true)
		FilledRectangle(ParentPos.x + Pos.x + 1, ParentPos.y + Pos.y + 1, Size.x - 2, Size.y - 2, Colour(255, 0, 0, 255));
	//FilledRectangle(Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 2, Toggle::ParentPos.y + Toggle::Pos.y - 1, Toggle::TextWidth + 4, Toggle::TextHeight, Colour(200, 0, 0, 255));
	Text(Toggle::Name, Toggle::ParentPos.x + Toggle::Pos.x + (Toggle::Size.x) + 3, Toggle::ParentPos.y + Toggle::Pos.y - (Toggle::Size.y / 8), 12,"Verdana", Colour(255, 255, 255, 255), None); 

}