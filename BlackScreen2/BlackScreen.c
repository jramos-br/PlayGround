#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CHAR:
        switch (wParam)
        {
        case '-':
            if (!IsIconic(hwnd))
                CloseWindow(hwnd);
            break;
        case 27: // ESCAPE
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        default:
            MessageBeep(MB_ICONASTERISK);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    LPCTSTR lpszClassName = TEXT("BlackScreen Window Class");
    LPCTSTR lpszWindowName = TEXT("BlackScreen");
    LPCTSTR lpszGetMonitorInfoError = TEXT("Could not retrieve information about the primary display monitor.");
    LPCTSTR lpszRegisterClassError = TEXT("Could not register the window class.");
    LPCTSTR lpszCreateWindowError = TEXT("Could not create the main window.");

    // Get the window position and size.

    POINT ptZero = {0, 0};
    HMONITOR hmon = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi;
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(mi);

    if (!GetMonitorInfo(hmon, &mi))
    {
        MessageBox(NULL, lpszGetMonitorInfoError, NULL, MB_ICONERROR);
        return 0;
    }

    int x = mi.rcMonitor.left;
    int y = mi.rcMonitor.top;
    int nWidth = mi.rcMonitor.right - mi.rcMonitor.left;
    int nHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;

    // Register the window class.

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = lpszClassName;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, lpszRegisterClassError, NULL, MB_ICONERROR);
        return 0;
    }

    // Create the window.

    DWORD dwStyle = WS_POPUP;
    HWND hwndParent = NULL;
    HMENU hMenu = NULL;
    LPVOID lpParam = NULL;

    HWND hwnd = CreateWindow(lpszClassName, lpszWindowName, dwStyle,
                             x, y, nWidth, nHeight,
                             hwndParent, hMenu, hInstance, lpParam);
    if (hwnd == NULL)
    {
        MessageBox(NULL, lpszCreateWindowError, NULL, MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
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

    // Unregister the window class.

    UnregisterClass(lpszClassName, hInstance);

    // Exit program.

    return (int)msg.wParam;
}

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
