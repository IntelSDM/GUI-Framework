#pragma once
void CreateFonts(std::string customfontname, std::wstring_view fontname, float size, DWRITE_FONT_WEIGHT weight);
void GetTextSize(const std::string text, float* const width, float* const height, std::string font);
IDWriteTextFormat* GetFont(std::string fontname);
enum FontAlignment
{
    Left = 0,
    Centre = 1,
    Right = 2,
    None = 3
};