#include "pch.h"
#include "Font.h"

std::map<std::string, IDWriteTextFormat*> Fonts;

void CreateFonts(std::string customfontname, std::wstring_view fontname, float size, DWRITE_FONT_WEIGHT weight)
{
    IDWriteTextFormat* text_format;
    HRESULT result = FontFactory->CreateTextFormat(
        fontname.data(),
        nullptr,
        weight,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        size,
        L"",
        &Fonts[customfontname]
    );
}

void GetTextSize(const std::string text, int fontsize,float* const width, float* const height, std::string font)
{

    if (!text.empty())
    {
        std::wstring convertedtext(text.begin(), text.end());
        if (!width && !height)
        {
            return;
        }

        IDWriteTextLayout* layout = nullptr;
        const HRESULT status = FontFactory->CreateTextLayout(convertedtext.data(), static_cast<std::uint32_t>(text.length()), Fonts[font], 4096.f, 4096.f, &layout);
        float modifier = layout->GetFontSize() / 4.0f;
        DWRITE_TEXT_RANGE range = DWRITE_TEXT_RANGE();
        range.length = text.length();
        range.startPosition = 0;
        layout->SetFontSize(fontsize, range);
        if (!SUCCEEDED(status))
        {
            layout->Release(); // free memory
            return;
        }
        DWRITE_TEXT_METRICS metrics{};
        if (SUCCEEDED(layout->GetMetrics(&metrics)))
        {
            if (width)
            {
                *width = metrics.width + modifier;
            }
            if (height)
            {
                *height = metrics.height;
            }
        }
        layout->Release();
        
    }
}

IDWriteTextFormat* GetFont(std::string fontname)
{
    return Fonts[fontname];
}
