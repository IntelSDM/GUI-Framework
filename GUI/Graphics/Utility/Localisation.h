#pragma once

enum class Languages
{
	English,
	Chinese,
};

extern void AddTranslation(Languages language, std::wstring english, std::wstring translation);
extern void AddTranslation(Languages language, std::vector<std::wstring> english, std::vector<std::wstring> translation);
extern std::wstring GetTranslation(std::wstring english);
extern void SetLanguage(Languages language);
extern Languages GetLanguage();
extern std::wstring GetFlagPath(Languages lang);