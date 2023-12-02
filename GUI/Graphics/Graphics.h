#pragma once

typedef struct FontInformation
{
	size_t size;
	IDWriteTextFormat* font;
};

extern ID2D1Factory* Factory;
extern ID2D1HwndRenderTarget* RenderTarget;
extern IDWriteFactory* FontFactory;
extern ID2D1SolidColorBrush* Brush;
extern std::map<std::string, FontInformation> Fonts;
extern HWND Hwnd;
