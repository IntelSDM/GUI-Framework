#pragma once
#include "Entity.h"

class Label : public Entity
{
protected:
    bool Tabbed;
    int TextSize = 11;
    std::string Font = "Verdana";
public:
    Label(const std::wstring& name, float x, float y);
    void Update();
    void Draw();
};

