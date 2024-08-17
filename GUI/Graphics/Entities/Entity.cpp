#include "pch.h"
#include "Entity.h"
#include "drawing.h"
#pragma region Entity

bool Entity::IsVisible()
{
	if (Entity::Condition)
		return Entity::Condition();

	return Entity::Visible;
}

void Entity::SetCondition(condition condition)
{
	Entity::Condition = condition;
}

void Entity::SetVisible(bool visible)
{
	Entity::Visible = visible;
}

void Entity::SetBlocked(bool block)
{
	Entity::Blocked = block;
}
void Entity::SetToolTip(std::wstring toolTip)
{
	Entity::ToolTip = toolTip;
}
void Entity::DrawTooltip()
{
	if (ToolTip.size() == 0)
		return;
	if (Blocked)
		return;
	if (IsMouseInRectangle(Pos + ParentPos, Size))
	{
		
		Vector2 tooltipsize = GetTextSize(L"⚠ " + ToolTip, "Verdana", 11);
		FilledRectangle(MousePos.x + 15, MousePos.y + 10, tooltipsize.x + 10, tooltipsize.y + 5, MyColour(20,20, 20, 150));
		DrawText(MousePos.x + 18, MousePos.y + 10, L"⚠ " + ToolTip, "Verdana", 11, MyColour(255, 255, 255, 255), None);

	}
}
void Entity::DrawTooltip(Vector2 start, Vector2 size)
{
	if (ToolTip.size() == 0)
		return;
	if (Blocked)
		return;
	if (IsMouseInRectangle(start + ParentPos, size))
	{

		Vector2 tooltipsize = GetTextSize(L"⚠ " + ToolTip, "Verdana", 11);
		FilledRectangle(MousePos.x + 15, MousePos.y + 10, tooltipsize.x + 10, tooltipsize.y + 5, MyColour(20, 20, 20, 150));
		DrawText(MousePos.x + 18, MousePos.y + 10, L"⚠ " + ToolTip, "Verdana", 11, MyColour(255, 255, 255, 255), None);

	}
}
child Entity::GetParent()
{
	return Entity::Parent;
}

child Entity::GetChildRelativeParent()
{
	return Entity::ChildRelativeParent;
}

void Entity::SetBlockedSiblings(bool block)
{
	for (auto& it : Entity::Parent->GetContainer())
	{
		if (it == shared_from_this())
			continue;

		it->SetBlocked(block);
	}
}

Entity* Entity::GetInstance()
{
	return Entity::Instance;
}

float Entity::GetLastClick()
{
	return Entity::LastClick;
}

void Entity::SetLastClick(float value)
{
	Entity::LastClick = value;
}

void Entity::SetParent(child parent)
{
	Entity::Parent = parent;
}

void Entity::SetRelativeParent(child parent)
{
	Entity::ChildRelativeParent = parent;
}
#pragma endregion

#pragma region Events

void Entity::SetValueChangedEvent(std::function<void()> evnt)
{
	Entity::ValueChangeEvent = evnt;
}

std::function<void()> Entity::GetValueChangedEvent()
{
	return Entity::ValueChangeEvent;
}
#pragma endregion

#pragma region Container
void Container::Draw()
{
	// Reverse iterat
	for (auto it = ContainerInstance.crbegin(); it != ContainerInstance.crend(); it++)
	{
		(*it)->Draw();
	}
}

void Container::Update()
{
	for (auto it = ContainerInstance.crbegin(); it != ContainerInstance.crend(); it++)
	{
		(*it)->Update();
	}
}

/*childcontainer& Container::Container()
{
    return Container::ContainerInstance;
}*/

/*size_t Container::GetChildren()
{
    return Container::ContainerInstance.size();
}*/
#pragma endregion
