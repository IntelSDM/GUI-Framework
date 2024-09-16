#pragma once
#include "Entity.h"

class DropDownTextBox : public Entity
{
protected:
	std::chrono::time_point<std::chrono::high_resolution_clock> AnimationStart;
	float AnimationInterval = 1.0f;

	std::chrono::time_point<std::chrono::high_resolution_clock> SelectingAnimationStart;
	float SelectingAnimationInterval = 0.005f;

	int SelectedPoint;
	float SelectedPosition;

	int SelectionStart;
	int SelectionEnd;
	float SelectingStartPosition = 0;
	float SelectingEndPosition = 0;

	int VisiblePointerStart;
	int VisiblePointerEnd;
	void SetStartIndex();


	std::wstring MainString;
	std::wstring VisibleString;

	float TextWidth = 0;

	bool ContextActive = false;
	Vector2 ContextPos;
	Vector2 ContextSize;
	void ContextCopyText();
	void ContextSelectAll();
	void ContextPasteText();
	std::unordered_map<std::wstring, std::function<void()>> ContextNames = {
		{L"Select All", [this]() { ContextSelectAll(); }},
		{L"Copy", [this]() { ContextCopyText(); }},
		{L"Paste", [this]() { ContextPasteText(); }}
	};

	bool Active = false;
	bool Selecting = false;
	bool Held = false;

	bool IsKeyAcceptable();

	void SetState();
	void ArrowKeyNavition();
	void InputText();
	void DeleteText();
	void SetSelectionPoint();
	void SelectionDragging();
	void CopyText();
	void PasteText();
	void ContextMenu();
	void SetSelection();


	int TextSize = 11;
	std::string Font = "Verdana";

	std::vector<std::wstring>Names;
	std::vector<std::wstring>VisibleNames;
	int* Index;
	void ConvertSelectedName();
	float CutOffBuffer = 0;
	void CalculateBuffer();
	float SizeDifference;
	float DropWidth;
	void SetDropDownWidth();
	bool HasTyped = false;

public:
	DropDownTextBox(float x, float y, std::wstring text, int* items, std::vector<std::wstring>names);
	void Update();
	void Draw();
	bool IsTabbed;
};
