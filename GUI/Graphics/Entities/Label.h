#pragma once
#include "Entity.h"

class Label : public Entity
{
protected:
    bool Tabbed;
    std::string Name;
public:
    Label(std::string, float, float);
    void Update();
    void Draw();
};

