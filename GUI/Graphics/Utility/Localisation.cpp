#include "pch.h"
#include "Localisation.h"

Languages Language = Languages::English;

// language, English, translation
std::unordered_map<Languages, std::unordered_map<std::wstring, std::wstring>> Translations;

void AddTranslation(Languages language, std::wstring english, std::wstring translation)
{

	Translations[language][english] = translation;
}

std::wstring GetTranslation(std::wstring english)
{
	if (Translations.find(Language) == Translations.end())
	{
		return english;
	}

	if (Translations[Language].find(english) == Translations[Language].end())
	{
		return english;
	}

	return Translations[Language][english];
}

void SetLanguage(Languages language)
{
	Language = language;
}

Languages GetLanguage()
{
	return Language;
}