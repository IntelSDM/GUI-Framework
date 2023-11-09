#pragma once
#include "Entity.h"
class Button : public Entity
{
protected:

    std::function< void()> Action;
    void SetWidth();
    std::chrono::time_point<std::chrono::high_resolution_clock> LastUpdateTime;
    D2D1::ColorF CurrentColour = Colour(100, 185, 255, 255);
    D2D1::ColorF InterpolateColour(const D2D1::ColorF& colour1, const D2D1::ColorF& colour2, float progress);
    const float HoverAnimationDuration = 0.5; // Duration of the color change animation in seconds
    float HoverAnimationProgress = 0.0;
    bool IsHovered = false;

public:
    Button(float x, float y, std::wstring text, std::function<void()> click);
    void Update();
    void Draw();
};
