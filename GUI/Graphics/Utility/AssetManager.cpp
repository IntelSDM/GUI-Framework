#include "pch.h"
#include "AssetManager.h"
#include "Assets/FormIcon.h"
#include <filesystem>
std::string AssetBasePath = getenv("USERPROFILE") + (std::string)("\\Documents\\GUIFramework\\");
std::string FormIconPath = AssetBasePath + "FormLogo.png";

std::wstring GetFormIconPath()
{
	std::string combined = FormIconPath;
	return std::wstring(combined.begin(), combined.end());
}

void WriteAssetToFile(std::string path, std::vector<uint8_t> asset)
{
	printf("Writing asset to file: %s\n", path.c_str());
	std::ofstream fout(path, std::ios::out | std::ios::binary);
	fout.write((char*)asset.data(), asset.size());
	fout.close();
}
void SetUpAssets()
{
	std::filesystem::create_directory(std::filesystem::path(getenv("USERPROFILE") + (std::string)("\\Documents\\")).string());
	std::filesystem::create_directory(std::filesystem::path(getenv("USERPROFILE") + (std::string)("\\Documents\\GUIFramework\\")).string());

	WriteAssetToFile(FormIconPath, FormIconBytes);
}