#include "pch.h"
#include "button.h"
#include "Input.h"
#include "Drawing.h"

Button::Button(float x, float y, std::string text, std::function<void()> click)
{
	Button::Pos = { x,y };
	Button::Size = { 70,60 };
	Button::Name = text;
	Button::Action = click;
	
}

void Button::Update()
{
	if (!Button::Parent)
		Button::SetVisible(false);
	if (!Button::IsVisible())
		return;


	Button::ParentPos = Button::Parent->GetParent()->GetPos();
	if (IsMouseInRectangle(Button::Pos + ParentPos, Button::Size) && IsKeyClicked(VK_LBUTTON) && Button::LastClickTime < (clock() * 0.00001f))
	{
		Button::Action();
		Button::LastClickTime = (clock() * 0.00001f) + 0.002f;
	}
}

void Button::Draw()
{
	//Button::Parent->GetParent()->GetParent()->GetPos();
	if (!Button::Parent)
		Button::SetVisible(false);
	if (!Button::IsVisible())
		return;

	FilledRectangle(ParentPos.x + Button::Pos.x, ParentPos.y + Button::Pos.y, Button::Size.x, Button::Size.y, Colour(20, 30, 160, 255));
	Text(Button::Name, ParentPos.x + Button::Pos.x + (Button::Size.x / 2), ParentPos.y + Button::Pos.y + (Button::Size.y / 2),12,"Verdana" ,Colour(240, 240, 240, 255), Centre);

}