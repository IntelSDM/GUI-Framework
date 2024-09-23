#pragma once
#include "Vector.h"

enum FontAlignment
{
	Left = 0,
	Centre = 1,
	Right = 2,
	CentreLeft = 3,
	CentreRight = 4,
	CentreCentre = 5,
	None = 6
};

extern void CreateFonts(const std::string& customfontname, const std::wstring_view& fontname, const float& size, const DWRITE_FONT_WEIGHT& weight);
extern Vector2 GetTextSize(const std::wstring& text, const std::string& font, size_t font_size = 0);
extern size_t GetFontSize(const std::string& fontname);
extern IDWriteTextFormat* GetFont(const std::string& fontname);
