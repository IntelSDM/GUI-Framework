#pragma once
#include "Entity.h"

class TabListBox : public Container
{
protected:
	int* Selected;

public:
	TabListBox(const std::wstring& name);
	int Index;
	void Update();
	void Draw();
};
