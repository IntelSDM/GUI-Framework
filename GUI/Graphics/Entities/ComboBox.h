#pragma once
#include "Entity.h"

class ComboBox : public Entity
{
protected:
	float DropWidth;
	std::vector<std::wstring> Names;
	std::vector<bool*> Items;
	bool Active = false;
	std::wstring SelectedName;
	float TextWidth = 0;
	float CutOffBuffer = 0;
	float SizeDifference;

	int MaxVisibleItems = 5;

	int PointerStart = 0;
	int PointerEnd = MaxVisibleItems;

	void ConvertSelectedName();
	void CalculateBuffer();
	void SetComboBoxWidth();
	void ArrowNavigation();
	void UpdateScrollBar();

	bool ScrollBarHeld = false;
	int TextSize = 11;
	std::string Font = "Verdana";
public:
	ComboBox(float x, float y, std::wstring text, std::vector<bool*> items, std::vector<std::wstring> names);
	void Update();
	void Draw();
};
