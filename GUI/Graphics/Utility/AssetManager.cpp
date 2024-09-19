#include "pch.h"
#include "AssetManager.h"
#include "Assets/FormIcon.h"
#include "Assets/map.h"
#include "Assets/CNFlag.h"
#include "Assets/UKFlag.h"
#include <filesystem>
std::string AssetBasePath = getenv("USERPROFILE") + (std::string)("\\Documents\\GUIFramework\\");
std::string FormIconPath = AssetBasePath + "FormLogo.png";
std::string MapPath = AssetBasePath + "Map.png";
std::string ChineseFlagPath = AssetBasePath + "CNFlag.png";
std::string UKFlagPath = AssetBasePath + "UKFlag.png";
std::wstring GetFormIconPath()
{
	std::string combined = FormIconPath;
	return std::wstring(combined.begin(), combined.end());
}
std::wstring GetMapPath()
{
	std::string combined = MapPath;
	return std::wstring(combined.begin(), combined.end());
}
std::wstring GetChineseFlagPath()
{
	std::string combined = ChineseFlagPath;
	return std::wstring(combined.begin(), combined.end());
}
std::wstring GetUKFlagPath()
{
	std::string combined = UKFlagPath;
	return std::wstring(combined.begin(), combined.end());
}
void WriteAssetToFile(const std::string& path, const unsigned char* asset, size_t assetSize)
{
    std::cout << "Writing asset to file: " << path << std::endl;
    std::ofstream fout(path, std::ios::out | std::ios::binary);
    if (!fout) 
        return;
    
    fout.write(reinterpret_cast<const char*>(asset), assetSize);
    if (!fout)
        return;
    
    fout.close();
    std::cout << "Asset successfully written to file." << std::endl;
}
void SetUpAssets()
{
	std::filesystem::create_directory(std::filesystem::path(getenv("USERPROFILE") + (std::string)("\\Documents\\")).string());
	std::filesystem::create_directory(std::filesystem::path(getenv("USERPROFILE") + (std::string)("\\Documents\\GUIFramework\\")).string());

	WriteAssetToFile(FormIconPath, FormIconBytes,sizeof(FormIconBytes));
	WriteAssetToFile(MapPath, MapBytes, sizeof(MapBytes));
    WriteAssetToFile(ChineseFlagPath, ChineseFlag, sizeof(ChineseFlag));
    WriteAssetToFile(UKFlagPath, UKFlag, sizeof(UKFlag));
}

