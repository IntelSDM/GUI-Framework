#include "pch.h"
#include "LoadingBar.h"
#include <filesystem>
#include "Drawing.h"
LoadingBar::LoadingBar(float x, float y, float width, float height, const std::wstring& text, int* value, const std::wstring& iconpath)
{
	LoadingBar::Name = text;
	Progress = value;
	LoadingBar::Pos = { x, y };
	LoadingBar::Size = { width, height };
	LoadingBar::SetVisible(true);
	LoadingBar::IconPath = iconpath;
	if (std::filesystem::exists(IconPath))
		CreateDirectXBitmap(IconPath, &Bitmap);
	SetVisible(true);
}

void LoadingBar::SetText(const std::wstring& text)
{
	LoadingBar::Name = text;
}
void LoadingBar::SetProgress(int value)
{
Progress = &value;
}
void LoadingBar::SetStatus(bool value)
{
	LoadingBar::Status = value;
}
void LoadingBar::Update()
{
	

	if (!LoadingBar::Parent)
		LoadingBar::SetVisible(false);
	LoadingBar::ParentPos = LoadingBar::Parent->GetParentPos();
	if (!LoadingBar::IsVisible())
		return;

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (LoadingBar::Status)
	{
		if (now >= NextAnimation) {
			if (LoadingBar::Rotation >= 360)
				LoadingBar::Rotation = 0;
			else
				LoadingBar::Rotation++;
			NextAnimation = now + AnimationDelay;
		}
	}
}


void LoadingBar::Draw()
{
	if (!LoadingBar::Parent)
		LoadingBar::SetVisible(false);
	if (!LoadingBar::IsVisible())
		return;


	MyColour textColour = MenuColours["Text"];
	std::string progress = std::to_string(*LoadingBar::Progress);
	std::wstring wprogress =  L"[" + std::wstring(progress.begin(), progress.end()) + L"%]";

	if (IconPath.size() == 0 || !Bitmap)
	{
		DrawText(LoadingBar::ParentPos.x + LoadingBar::Pos.x + (LoadingBar::Size.x / 2), LoadingBar::ParentPos.y + LoadingBar::Pos.y + 20 + (LoadingBar::Size.y), wprogress + GetTranslation(LoadingBar::Name), Font, TextSize, textColour, Centre);
	}
	else
	{
		DrawBitmap(LoadingBar::Bitmap, LoadingBar::ParentPos.x + LoadingBar::Pos.x, LoadingBar::ParentPos.y + LoadingBar::Pos.y, LoadingBar::Size.x, LoadingBar::Size.y, LoadingBar::Rotation);
		DrawText(LoadingBar::ParentPos.x + LoadingBar::Pos.x + (LoadingBar::Size.x / 2), LoadingBar::ParentPos.y + LoadingBar::Pos.y + 20 + (LoadingBar::Size.y), wprogress + GetTranslation(LoadingBar::Name), Font, TextSize, textColour, Centre);
	}
}