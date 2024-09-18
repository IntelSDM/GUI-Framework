#pragma once

enum class Languages
{
	English,
	Chinese,
	Russian

};

extern void AddTranslation(Languages language, std::wstring english, std::wstring translation);
extern std::wstring GetTranslation(std::wstring english);
extern void SetLanguage(Languages language);
extern Languages GetLanguage();