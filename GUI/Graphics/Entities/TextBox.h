#pragma once
#include "Entity.h"
class TextBox : public Entity
{
protected:

    int LastAnimationTime = 0;
    int AnimationDuration = 1;

    Vector2 Pos;
    Vector2 Size;
    Vector2 ParentPos;

    std::string* MainString;
    std::string VisibleString;

    float TextWidth = 0;
    float TextHeight = 0;

public:
    TextBox(float x, float y, std::string text, std::string* data);
    void Update();
    void Draw();
    bool IsTabbed;
};
