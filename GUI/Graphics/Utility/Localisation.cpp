#include "pch.h"
#include "Localisation.h"
#include "AssetManager.h"
#include <regex>

Languages Language = Languages::English;

// language, English, translation
std::unordered_map<Languages, std::unordered_map<std::wstring, std::wstring>> Translations;

void AddTranslation(Languages language, std::wstring english, std::wstring translation)
{

	Translations[language][english] = translation;
}

void AddTranslation(Languages language, std::vector<std::wstring> english, std::vector<std::wstring> translation)
{
	for (int i = 0; i < english.size(); i++)
	{
		Translations[language][english[i]] = translation[i];
	}
}


std::wstring GetTranslation(std::wstring english)
{
    if (Language == Languages::English)
    {
		return english;
	}
    if (Translations.find(Language) == Translations.end())
    {
        return english;
    }
    if (Translations[Language].find(english) == Translations[Language].end())
    {

        std::wregex regex(L"(.*?)(\\d+)$");
        std::wsmatch match;

        if (std::regex_match(english, match, regex))
        {

            std::wstring wordPart = match[1].str();
            std::wstring numberPart = match[2].str();

            if (Translations[Language].find(wordPart) != Translations[Language].end())
            {

                std::wstring translatedString = Translations[Language][wordPart] + numberPart;


                Translations[Language][english] = translatedString;

                return translatedString;
            }
            else
            {

                return english;
            }
        }


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

std::wstring GetFlagPath(Languages lang)
{
    switch (lang)
    {
	case Languages::English:
		return GetUKFlagPath();
	case Languages::Chinese:
		return GetChineseFlagPath();
	}
	return L"";
}