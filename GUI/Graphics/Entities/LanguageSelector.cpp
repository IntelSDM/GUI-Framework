#include "pch.h"
#include "Input.h"
#include "Drawing.h"
#include "LanguageSelector.h"
#include "Font.h";
#include "Graphics.h"
#include "localisation.h"

LanguageSelector::LanguageSelector(float x, float y)
{
	LanguageSelector::Pos = { x, y };
	LanguageSelector::Size = { 30, 30 };
	SetVisible(true);
	CreateDirectXBitmap(GetFlagPath(Languages::English), &LanguageImages[Languages::English]);
	CreateDirectXBitmap(GetFlagPath(Languages::Chinese), &LanguageImages[Languages::Chinese]);
}

void LanguageSelector::Update()
{
	if (!LanguageSelector::Parent)
		LanguageSelector::SetVisible(false);
	LanguageSelector::ParentPos = LanguageSelector::Parent->GetParentPos();
	if (!LanguageSelector::IsVisible())
		return;

	if (!LanguageSelector::Blocked)
	{
		if (IsMouseInRectangle(LanguageSelector::Pos + ParentPos, LanguageSelector::Size))
		{
			if (IsKeyDown(VK_LBUTTON))
			{
				LanguageSelector::Active = true;
				SetBlockedSiblings(true);
				

			}
		}

	}
	if (!IsMouseInRectangle(LanguageSelector::Pos + ParentPos, LanguageSelector::Size) && !IsMouseInRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4), ParentPos.y + Pos.y + Size.y, Size.x * LanguageImages.size(), Size.y) && IsKeyClicked(VK_LBUTTON))
	{
		Active = false;
		SetBlockedSiblings(false);
	}
	if (Active)
	{
		int i = 0;
		for (auto& image : LanguageImages)
		{
			if (IsMouseInRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4) + (Size.x * i), ParentPos.y + Pos.y + Size.y, Size.x, Size.y))
			{
				if (IsKeyClicked(VK_LBUTTON))
				{
					SetLanguage(image.first);
					Active = false;
					SetBlockedSiblings(false);
				}

			}
			i++;
		}
	}
}

void LanguageSelector::Draw()
{
	if (!LanguageSelector::Parent)
		LanguageSelector::SetVisible(false);
	if (!LanguageSelector::IsVisible())
		return;





	OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, Colour(255,255,255));
	DrawBitmap(LanguageImages[GetLanguage()],ParentPos.x + Pos.x, ParentPos.y + Pos.y,Size.x,Size.y);
	if (IsMouseInRectangle(LanguageSelector::Pos + ParentPos, LanguageSelector::Size))
	{
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, Colour(100, 100, 100, 100));
	}
	if (Active)
	{
		OutlineRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4), ParentPos.y + Pos.y + Size.y, (Size.x * LanguageImages.size())+1, Size.y +1,1, Colour(255, 255, 255, 255));
	FilledRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size())/4), ParentPos.y + Pos.y + Size.y, Size.x * LanguageImages.size(), Size.y, Colour(20, 20, 20, 255));
	int i = 0;
		for (auto& image : LanguageImages)
		{
			DrawBitmap(image.second, (ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4) + (Size.x * i), ParentPos.y + Pos.y + Size.y, Size.x, Size.y);
			if(IsMouseInRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4) + (Size.x * i), ParentPos.y + Pos.y + Size.y, Size.x, Size.y))
			{
								FilledRectangle((ParentPos.x + Pos.x) - ((Size.x * LanguageImages.size()) / 4) + (Size.x * i), ParentPos.y + Pos.y + Size.y, Size.x, Size.y, Colour(100, 100, 100, 100));
			}
			i++;
		}
	}
}
