#pragma once
#include "Entity.h"


class LoadingBar : public Entity
{
protected:

	ID2D1Bitmap* Bitmap = nullptr;
	std::wstring IconPath;
	float Rotation = 0;
	int* Progress;
	std::chrono::milliseconds AnimationDelay = std::chrono::milliseconds(15);
	std::chrono::steady_clock::time_point NextAnimation = std::chrono::steady_clock::now();
	bool Status = true;
public:
	LoadingBar(float x, float y,float width, float height, const std::wstring& text, int* value,const std::wstring& iconpath);
	void SetText(const std::wstring& text);
	void Update();
	void Draw();
	void SetProgress(int value);
	void SetStatus(bool status);
};

