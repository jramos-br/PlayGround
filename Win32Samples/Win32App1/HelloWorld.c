#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LPCTSTR lpszClassName = TEXT("HelloWorld Window Class");
    LPCTSTR lpszWindowName = TEXT("HelloWorld");

    // Register the window class.

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = lpszClassName;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles
        lpszClassName,       // Window class
        lpszWindowName,      // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    BOOL bRet;
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            msg.wParam = FALSE;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#pragma comment(lib, "User32.lib")
