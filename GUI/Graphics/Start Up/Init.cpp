#include "pch.h"
#include "Init.h"

ID2D1Factory* Factory;
IDWriteFactory* FontFactory;
ID2D1HwndRenderTarget* RenderTarget;
ID2D1SolidColorBrush* Brush;

void InitD2D(HWND hWnd)
{


    HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &Factory);
    RECT rect;
    GetClientRect(hWnd, &rect);
    result = Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &RenderTarget);
    if (SUCCEEDED(result))
    {
        result = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_ISOLATED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&FontFactory)
        );
    }
}

void CleanD2D()
{
    // ensure pointer is valid
    // then release memory
    if(Factory)
    Factory->Release();
    if(RenderTarget)
    RenderTarget->Release();
    if(Brush)
    Brush->Release();
   
}
int FrameRate() {
    static int fps;
    static int lastfps;
    static float lasttime;
    static float time;


    time = clock() * 0.001f;
    fps++;
    float DeltaTime = time - lasttime;
    if ((DeltaTime) >= 1.0f) {
        lasttime = time;
        lastfps = fps;
        fps = 0;
    }
    return lastfps;
}

void RenderFrame()
{

    // begin a new backbuffer
    RenderTarget->BeginDraw();
    RenderTarget->Clear(Colour(0, 0, 0, 255)); // clear over the last buffer
    RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); // set new transform


    RenderTarget->EndDraw();

}
