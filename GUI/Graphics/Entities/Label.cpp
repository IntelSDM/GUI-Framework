#include "pch.h"
#include "Drawing.h"
#include "label.h"

Label::Label(std::string text, float posx, float posy)
{
	Label::Pos = { posx,posy };
	Label::Name = text;
}
void Label::Update()
{
	Label::ParentPos = Label::Parent->GetParent()->GetPos();
}
void Label::Draw()
{
	Text(Label::Name, Label::ParentPos.x + Label::Pos.x, Label::ParentPos.y + Label::Pos.y, 12, "Verdana", Colour(255, 255, 255, 255), None);
}