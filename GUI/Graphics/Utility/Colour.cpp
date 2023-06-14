#include "pch.h"
#include "Colour.h"
D2D1::ColorF Colour(UINT8 R, UINT8 G, UINT8 B, UINT8 A)
{
    return D2D1::ColorF(static_cast<float>(R) / 255.0f, static_cast<float>(G) / 255.0f, static_cast<float>(B) / 255.0f, static_cast<float>(A) / 255.0f);

}