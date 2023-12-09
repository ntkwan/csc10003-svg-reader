// clang-format off
#include <winsock2.h>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
// clang-format on

#include "Parser.hpp"
#include "Viewer.hpp"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Parser* parser = nullptr;

void OnPaint(HDC hdc, const std::string& filePath, Viewer& viewer) {
    Gdiplus::Graphics graphics(hdc);
    if (!parser) {
        parser = Parser::getInstance(filePath);
    }
    graphics.RotateTransform(viewer.rotateAngle);
    graphics.ScaleTransform(viewer.zoomFactor, viewer.zoomFactor);
    graphics.TranslateTransform(viewer.offsetX, viewer.offsetY);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
    graphics.SetTextContrast(100);
    graphics.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
    graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQuality);
    Renderer* renderer = Renderer::getInstance();
    SVGElement* root = parser->getRoot();
    Group* group = dynamic_cast< Group* >(root);
    renderer->draw(graphics, group);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {
    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("svg-reader-v0.2");

    RegisterClass(&wndClass);

    hWnd = CreateWindowEx(0,
                          TEXT("svg-reader-v0.2"),  // window class name
                          TEXT("svg-reader-v0.2"),  // window caption
                          WS_OVERLAPPEDWINDOW,      // window style
                          CW_USEDEFAULT,            // initial x position
                          CW_USEDEFAULT,            // initial y position
                          1600,                     // initial x size
                          1200,                     // initial y size
                          NULL,                     // parent window handle
                          NULL,                     // window menu handle
                          hInstance,                // program instance handle
                          NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (parser) delete parser;
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    std::string filePath;
    if (__argc > 1) {
        filePath = __argv[1];
    }
    if (!parser) {
        parser = Parser::getInstance(filePath);
    }
    Viewer* viewer = Viewer::getInstance();
    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            OnPaint(hdc, filePath, *viewer);
            EndPaint(hWnd, &ps);
            return 0;
        case WM_MOUSEWHEEL:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            viewer->handleMouseEvent(message, wParam, lParam);
            if (viewer->needsRepaint) {
                InvalidateRect(hWnd, NULL, TRUE);
                viewer->needsRepaint = false;
            }
            return 0;
        case WM_KEYDOWN:
            viewer->handleKeyEvent(wParam);
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}