#include "pch.h"
#include "Drawing.h"

void Text(std::string text, int x, int y, int fontsize, std::string font, D2D1::ColorF colour, FontAlignment alignment)
{
    Brush->SetColor(colour);

    std::wstring convertedtext(text.begin(), text.end()); // convert to unicode
    IDWriteTextLayout* layout = nullptr;
    const HRESULT status = FontFactory->CreateTextLayout(convertedtext.data(), static_cast<std::uint32_t>(text.length()), Fonts[font], 4096.f, 4096.f, &layout);
    if (!SUCCEEDED(status))
    {
        layout->Release(); // free memory
        return;
    }

    // set up text range
    DWRITE_TEXT_RANGE range = DWRITE_TEXT_RANGE();
    range.length = text.length();
    range.startPosition = 0;
    layout->SetFontSize(fontsize, range);
    DWRITE_TEXT_METRICS metrics{};
    layout->GetMetrics(&metrics);

    float modifier = layout->GetFontSize() / 4.0f; // metrics isn't ever correct
    D2D1_POINT_2F point; // position point
    // set the position
    switch (alignment)
    {
    case FontAlignment::Centre:
        x -= ((metrics.width + modifier) / 2);
        break;
    case FontAlignment::Right:
        x += ((metrics.width + modifier));
        break;
    case FontAlignment::Left:
        x -= ((metrics.width + modifier));
        break;
    case FontAlignment::None:
        break;
    case FontAlignment::CentreCentre:
        x -= ((metrics.width + modifier) / 2);
        y -= ((metrics.height + modifier) / 2);
        break;
    case FontAlignment::CentreLeft:
        x -= ((metrics.width + modifier));
        y += ((metrics.height + modifier) / 2);
        break;
    case FontAlignment::CentreRight:
        x += ((metrics.width + modifier));
        y -= ((metrics.height + modifier) / 2);
        break;
    }
    point.x = x;
    point.y = y;



    // draw
    RenderTarget->DrawTextLayout(point, layout, Brush, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_NONE);
    layout->Release(); // free memory
}

void TextClipped(std::string text, int x, int y, int width,int height,int fontsize, std::string font, D2D1::ColorF colour, FontAlignment alignment)
{
    Brush->SetColor(colour);

    std::wstring convertedtext(text.begin(), text.end()); // convert to unicode
    IDWriteTextLayout* layout = nullptr;
    const HRESULT status = FontFactory->CreateTextLayout(convertedtext.data(), static_cast<std::uint32_t>(text.length()), Fonts[font], width, height, &layout);
    if (!SUCCEEDED(status))
    {
        layout->Release(); // free memory
        return;
    }

    // set up text range
    DWRITE_TEXT_RANGE range = DWRITE_TEXT_RANGE();
    range.length = text.length();
    range.startPosition = 0;
    layout->SetFontSize(fontsize, range);
    DWRITE_TEXT_METRICS metrics{};
    layout->GetMetrics(&metrics);

    float modifier = layout->GetFontSize() / 4.0f; // metrics isn't ever correct
    D2D1_POINT_2F point; // position point
    // set the position
    switch (alignment)
    {
    case FontAlignment::Centre:
        x -= ((metrics.width + modifier) / 2);
        break;
    case FontAlignment::Right:
        x += ((metrics.width + modifier));
        break;
    case FontAlignment::Left:
        x -= ((metrics.width + modifier));
        break;
    case FontAlignment::None:
        break;
    case FontAlignment::CentreCentre:
        x -= ((metrics.width + modifier) / 2);
        y -= ((metrics.height + modifier) / 2);
        break;
    case FontAlignment::CentreLeft:
        x -= ((metrics.width + modifier));
        y += ((metrics.height + modifier) / 2);
        break;
    case FontAlignment::CentreRight:
        x += ((metrics.width + modifier));
        y -= ((metrics.height + modifier) / 2);
        break;
    }
    point.x = x;
    point.y = y;



    // draw
    RenderTarget->DrawTextLayout(point, layout, Brush, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);
    layout->Release(); // free memory
}