#pragma once
extern ID2D1Factory* Factory;
extern ID2D1HwndRenderTarget* RenderTarget;
extern IDWriteFactory* FontFactory;
extern ID2D1SolidColorBrush* Brush;
extern std::map<std::string, IDWriteTextFormat*> Fonts;
extern HWND Hwnd;