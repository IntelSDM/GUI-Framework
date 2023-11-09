#pragma once
D2D1::ColorF Colour(UINT8 R, UINT8 G, UINT8 B, UINT8 A = 255);
struct HsvColour
{
    float H;
    float S;
    float V;
};
D2D1::ColorF HueToRGB(float hue);
float RGBToHue(int r, int g, int b);
D2D1::ColorF HsvToRgb(float hue, int saturation, int value, int alpha);
HsvColour RgbToHsv(int r, int g, int b);
