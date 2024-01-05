/*
LPSTR = char*
LPCSTR = const char*
LPWSTR = wchar_t*
LPCWSTR = const wchar_t*
LPTSTR = char* or wchar_t* depending on _UNICODE
LPCTSTR = const char* or const wchar_t* depending on _UNICODE
*/

// For smaller header files
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
// Add unicode support
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>

LRESULT CALLBACK Win32MainWindowCallback(
    HWND Window, 
    UINT Message, 
    WPARAM WParam, 
    LPARAM LParam
) {

    switch (Message)
    {
        case WM_PAINT: {
            HDC DeviceContext;
            PAINTSTRUCT Paint;
            DeviceContext = BeginPaint(Window, &Paint);

            EndPaint(Window, &Paint);
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        case WM_CLOSE: {
            PostQuitMessage(0);
        } break;
        default: {
            return DefWindowProc(Window, Message, WParam, LParam);
        } break;
    }

    return 0;
}

int CALLBACK WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR     CmdLine,
    int       ShowCmd
) {
// int main(void) {
    WNDCLASSEX WinClass = { 0 };

    WinClass.cbSize = sizeof(WNDCLASSEX);
    WinClass.style = CS_HREDRAW | CS_VREDRAW;
    WinClass.lpfnWndProc = Win32MainWindowCallback;
    WinClass.hInstance = Instance;
    WinClass.hIcon = LoadIcon(WinClass.hInstance, IDI_APPLICATION);
    WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WinClass.lpszClassName = L"DesktopApp";
    WinClass.hIconSm = LoadIcon(WinClass.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&WinClass)) {
        return 1;
    }

    HWND Window = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        WinClass.lpszClassName,
        WinClass.lpszClassName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        Instance,
        NULL
    );

    if (!Window) {
        return 1;
    }

    ShowWindow(Window, ShowCmd);
    UpdateWindow(Window);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void __cdecl WinMainCRTStartup(void) {
    HINSTANCE Instance = GetModuleHandle(0);
    int exitCode = WinMain(Instance, 0, NULL, SW_SHOW);
	ExitProcess(exitCode);
}