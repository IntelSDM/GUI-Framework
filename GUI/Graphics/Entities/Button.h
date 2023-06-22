#pragma once
#include "Entity.h"
class Button : public Entity
{
protected:

    Vector2 Pos;
    Vector2 Size;
    Vector2 ParentPos;

    float LastClickTime = 0;

    std::function< void()> Action;

    std::string Name;
public:
    Button(float x, float y, std::string text, std::function<void()> click);
    void Update();
    void Draw();
};
