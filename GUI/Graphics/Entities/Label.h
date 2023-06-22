#pragma once
#include "Entity.h"

class Label : public Entity
{
protected:
    bool Tabbed;
public:
    Label(std::string, float, float);
    void Update();
    void Draw();
};

