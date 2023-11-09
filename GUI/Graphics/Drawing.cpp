#include "pch.h"
#include "Drawing.h"
ID2D1LinearGradientBrush* LinearBrush;
ID2D1GradientStopCollection* GradientStops = NULL;
void DrawText(std::string text, int x, int y, int fontsize, std::string font, D2D1::ColorF colour, FontAlignment alignment)
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

void OutlineCircle(int x, int y, float radius, float linewidth, D2D1::ColorF colour)
{
    Brush->SetColor(colour);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    RenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), Brush, linewidth);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}
void FilledCircle(int x, int y, float radius, D2D1::ColorF colour)
{
    Brush->SetColor(colour);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), Brush);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}
void OutlineRectangle(int x, int y, int width, int height, int linewidth, D2D1::ColorF colour)
{
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    Brush->SetColor(colour);
    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };
    RenderTarget->DrawRectangle(rect, Brush, linewidth);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

}
void FilledRectangle(int x, int y, int width, int height, D2D1::ColorF colour)
{
    Brush->SetColor(colour);
    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };
    RenderTarget->FillRectangle(rect, Brush);
}
void OutlineRoundedRectangle(int x, int y, int width, int height, int linewidth, int rounding, D2D1::ColorF colour)
{
    // RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    Brush->SetColor(colour);
    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
        rect,
        static_cast<float>(rounding),
        static_cast<float>(rounding)
    );
    RenderTarget->DrawRoundedRectangle(roundedRect, Brush, linewidth);
    //RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}
void FilledRoundedRectangle(int x, int y, int width, int height, int rounding, D2D1::ColorF colour)
{
    Brush->SetColor(colour);
    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
        rect,
        static_cast<float>(rounding),
        static_cast<float>(rounding)
    );
    RenderTarget->FillRoundedRectangle(roundedRect, Brush);
}
void FilledLine(int xstart, int ystart, int xend, int yend, int width, D2D1::ColorF colour)
{
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    D2D1_POINT_2F start = { static_cast<float>(xstart), static_cast<float>(ystart) };
    D2D1_POINT_2F finish = { static_cast<float>(xend), static_cast<float>(yend) };
    Brush->SetColor(colour);
    RenderTarget->DrawLine(start, finish, Brush);
}
// allows you to draw single lines, rather than being forced to use double
void FilledLineAliased(int xstart, int ystart, int xend, int yend, int width, D2D1::ColorF colour)
{
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
    D2D1_POINT_2F start = { static_cast<float>(xstart), static_cast<float>(ystart) };
    D2D1_POINT_2F finish = { static_cast<float>(xend), static_cast<float>(yend) };
    Brush->SetColor(colour);
    RenderTarget->DrawLine(start, finish, Brush);
    RenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}
void SaturationSlider(int x, int y, int width, int height, D2D1::ColorF colour)
{
    {
        D2D1_GRADIENT_STOP stops[] =
        {
        { 0.0f, colour },
        { 1.0f,D2D1::ColorF::Black },
        };
        RenderTarget->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(
                D2D1::Point2F(x + width, y),
                D2D1::Point2F(x, y + height)),
            GradientStops,
            &LinearBrush
        );

        D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };

        RenderTarget->FillRectangle(rect, LinearBrush);
        GradientStops->Release();
        LinearBrush->Release();
    }
    {
        D2D1_GRADIENT_STOP stops[] =
        {
            { 0.00f,Colour(255,255,255,200) },
            { 0.15f,Colour(colour.r,colour.g,colour.b,100) },
            { 1.0f, Colour(0,0,0,100) },
            { 0.6f, Colour(40,40,40,100) },
        };
        RenderTarget->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(
                D2D1::Point2F(x, y),
                D2D1::Point2F(x, y + height)),
            GradientStops,
            &LinearBrush
        );

        D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };

        RenderTarget->FillRectangle(rect, LinearBrush);
        GradientStops->Release();
        LinearBrush->Release();
    }
}
void AlphaSlider(int x, int y, int width, int height, D2D1::ColorF col)
{

    D2D1_GRADIENT_STOP stops[] =
    {
        { 0.0f,Colour(col.r,col.g,col.b,255) },
        { 1.0f,Colour(col.r,col.g,col.b,0) },
    };
    RenderTarget->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            D2D1::Point2F(x, y),
            D2D1::Point2F(x, y + height)),
        GradientStops,
        &LinearBrush
    );

    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };

    RenderTarget->FillRectangle(rect, LinearBrush);
    GradientStops->Release();
    LinearBrush->Release();

}

void HueSlider(int x, int y, int width, int height)
{
    D2D1_GRADIENT_STOP stops[] =
    {
        { 0.00f, Colour(255,0,0,255) },
        { 0.00f, Colour(255,0,0,255) },
        { 0.16f, Colour(255,255,0,255) },
        { 0.32f,  Colour(0,255,0,255) },
        { 0.48f,  Colour(0,255,255,255) },
        { 0.64f,  Colour(0,0,255,255) },
        { 0.80f,  Colour(255,0,255,255) },
        { 0.96f,  Colour(255,0,0,255) },
    };

    RenderTarget->CreateLinearGradientBrush(
        D2D1::LinearGradientBrushProperties(
            D2D1::Point2F(x, y),
            D2D1::Point2F(x + width, y)),
        GradientStops,
        &LinearBrush
    );

    D2D1_RECT_F rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y) };

    RenderTarget->FillRectangle(rect, LinearBrush);
    GradientStops->Release();
    LinearBrush->Release();


}
// draws at native resolution
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y)
{
    
    RenderTarget->DrawBitmap(bmp, D2D1::RectF(static_cast<float>(x), static_cast<float>(y), bmp->GetSize().width + x, bmp->GetSize().height + y), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height));

}
// squeezes, compresses or expands the image to set variables.
void DrawBitmap(ID2D1Bitmap* bmp, int x, int y, int width, int height)
{

    RenderTarget->DrawBitmap(bmp, D2D1::RectF(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width + x), static_cast<float>(height + y)), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height));

}