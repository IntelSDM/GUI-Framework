#pragma once
void Text(std::string text, int x, int y, int fontsize, std::string font, D2D1::ColorF colour, FontAlignment alignment);
void TextClipped(std::string text, int x, int y, int width, int height, int fontsize, std::string font, D2D1::ColorF colour, FontAlignment alignment);