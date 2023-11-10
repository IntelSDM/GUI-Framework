#include "pch.h"
#include "TabController.h"
TabController::TabController()
{
	TabController::SetVisible(true);
}

void TabController::Update()
{
	if (!TabController::Parent)
		TabController::SetVisible(false);

	if (!TabController::IsVisible())
		return;
	TabController::ParentPos = TabController::Parent->GetParentPos();
	TabController::Pos.x = TabController::Parent->GetPos().x;
	TabController::Pos.y = TabController::Parent->GetPos().y;
	TabController::Size = TabController::Parent->GetSize();
	TabController::Container::Update();
}

void TabController::Draw()
{
	if (!TabController::IsVisible())
		return;
	int size = TabController::Size.x / 12;

	TabController::Container::Draw();
}