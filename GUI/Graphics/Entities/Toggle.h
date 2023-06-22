#pragma once
#include "Entity.h"
class Toggle : public Entity
{
protected:
    float LastClickTime = 0;

    std::string Name;

    bool* Data;
    bool Tabbed = false;

    float TextWidth = 0;
    float TextHeight = 0;

public:
    Toggle(float x, float y, std::string text, bool* data);
    void Update();
    void Draw();
};
