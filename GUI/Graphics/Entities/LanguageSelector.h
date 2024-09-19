#pragma once
#include "Entity.h"
#include "Localisation.h"
#include "Drawing.h"
class LanguageSelector : public Entity
{
protected:
	int TextSize = 11;
	std::string Font = "Verdana";
	std::map<Languages, ID2D1Bitmap*> LanguageImages;
	bool Active = false;
public:
	LanguageSelector(float x, float y);
	void Update();
	void Draw();
};
