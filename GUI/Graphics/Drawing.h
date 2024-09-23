#pragma once
enum class LinearBrushStyle
{
	Downwards,
	Upwards,
	Leftwards,
	Rightwards
};
extern void DrawText(int x, int y, const std::wstring& text, const std::string& font, const int& fontsize, MyColour colour, const FontAlignment& alignment);
extern void OutlineCircle(const int& x, const int& y, const float& radius, const float& linewidth, MyColour colour);
extern void FilledCircle(const int& x, const int& y, const float& radius, MyColour colour);
extern void OutlineRectangle(const int& x, const int& y, const int& width, const int& height, const int& linewidth, MyColour colour);
extern void FilledRectangle(const int& x, const int& y, const int& width, const int& height, MyColour colour);
extern void OutlineRoundedRectangle(const int& x, const int& y, const int& width, const int& height, const int& linewidth, const int& rounding, MyColour colour);
extern void FilledRoundedRectangle(const int& x, const int& y, const int& width, const int& height, const int& rounding, MyColour colour);
extern void FilledLine(const int& xstart, const int& ystart, const int& xend, const int& yend, const int& width, MyColour colour);
extern void FilledLineAliased(const int& xstart, const int& ystart, const int& xend, const int& yend, const int& width, MyColour colour);
extern void DrawBitmap(ID2D1Bitmap* bmp, const int& x, const int& y, const int& width, const int& height);
extern void DrawBitmap(ID2D1Bitmap* bmp, const int& x, const int& y);
extern void SaturationSlider(const int& x, const int& y, const int& width, const int& height, MyColour colour);
extern void AlphaSlider(const int& x, const int& y, const int& width, const int& height, MyColour col);
extern void HueSlider(const int& x, const int& y, const int& width, const int& height);
extern void FilledTriangle(const int& x1, const int& y1, const int& x2, const int& y2, const int& x3, const int& y3, MyColour colour);
extern void CreateDirectXBitmap(const std::wstring& filename, ID2D1Bitmap** bmp);
extern void DrawBitmap(ID2D1Bitmap* bmp, const int& x, const int& y, const int& width, const int& height, const float& rotation);
extern void DrawBitmap(ID2D1Bitmap* bmp, const int& x, const int& y, const int& width, const int& height, float imageposx, float imageposy, float imagewidth, float imageheight);
extern void EndLayer();
extern void CreateLayer(const float& alpha);
extern void CreateLayer(const int& startx, const int& starty, const int& limitwidth, const int& limitheight);
extern void CreateLayer(const int& startx, const int& starty, const int& limitwidth, const int& limitheight, const float& alpha);
extern void FilledRectangle(const int& x, const int& y, const int& width, const int& height, ID2D1LinearGradientBrush* brush, const LinearBrushStyle& style);
extern void CreateLinearBrush(ID2D1LinearGradientBrush** brush, const float& point1, MyColour colour1, const float& point2, MyColour colour2, const Vector2& start, const Vector2& end);
extern void CreateLinearBrush(ID2D1LinearGradientBrush** brush, const float& point1, MyColour colour1, const float& point2, MyColour colour2, const float& point3, MyColour colour3, const Vector2& start, const Vector2& end);