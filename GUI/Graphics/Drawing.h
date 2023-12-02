#pragma once
void DrawText(int x, int y, std::wstring text, std::string font, int fontsize, D2D1::ColorF colour, FontAlignment alignment);
void OutlineCircle(int x, int y, float radius, float linewidth, D2D1::ColorF colour);
void FilledCircle(int x, int y, float radius, D2D1::ColorF colour);
void OutlineRectangle(int x, int y, int width, int height, int linewidth, D2D1::ColorF colour);
void FilledRectangle(int x, int y, int width, int height, D2D1::ColorF colour);
void OutlineRoundedRectangle(int x, int y, int width, int height, int linewidth, int rounding, D2D1::ColorF colour);
void FilledRoundedRectangle(int x, int y, int width, int height, int rounding, D2D1::ColorF colour);
void FilledLine(int xstart, int ystart, int xend, int yend, int width, D2D1::ColorF colour);
void FilledLineAliased(int xstart, int ystart, int xend, int yend, int width, D2D1::ColorF colour);
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y, int width, int height);
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y);
void SaturationSlider(int x, int y, int width, int height, D2D1::ColorF colour);
void AlphaSlider(int x, int y, int width, int height, D2D1::ColorF col);
void HueSlider(int x, int y, int width, int height);
void FilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, D2D1::ColorF colour);
