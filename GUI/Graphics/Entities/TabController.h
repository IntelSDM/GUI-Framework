#pragma once
#include "Entity.h"
class TabController : public Container
{
protected:
	Vector2 SetUpPos;
public:
	TabController();
	void Update();
	void Draw();
};