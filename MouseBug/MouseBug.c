#include <Windows.h>
#include <strsafe.h>

#define WMAPP_NOTIFYCALLBACK (WM_APP + 1)
#define NOTIFICATIONICONID 1

#define IDM_RESET 1
#define IDM_EXIT 2

BOOL AddNotificationIcon(HWND hwnd)
{
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = NOTIFICATIONICONID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    StringCchCopy(nid.szTip, _countof(nid.szTip), TEXT("MouseBug"));
    return Shell_NotifyIcon(NIM_ADD, &nid);
}

BOOL DeleteNotificationIcon(HWND hwnd)
{
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = NOTIFICATIONICONID;
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowContextMenu(HWND hwnd)
{
    POINT pt;
    GetCursorPos(&pt);
    HMENU hMenu = CreatePopupMenu();
    if (hMenu != NULL)
    {
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset aaaaaaaaaa aaaaaaaaaaaaaaaaaaaaa"));
        AppendMenu(hMenu, MF_STRING, IDM_RESET, TEXT("Reset"));
        AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("Exit"));
        SetForegroundWindow(hwnd);
        UINT uFlags = TPM_BOTTOMALIGN | TPM_RIGHTBUTTON;
        if (GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0)
            uFlags |= TPM_LEFTALIGN | TPM_HORPOSANIMATION | TPM_VERNEGANIMATION;
        else
            uFlags |= TPM_RIGHTALIGN | TPM_HORNEGANIMATION | TPM_VERNEGANIMATION;
        TrackPopupMenu(hMenu, uFlags, pt.x, pt.y, 0, hwnd, NULL);
        DestroyMenu(hMenu);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (uMsg)
    {
    case WM_CREATE:
        if (!AddNotificationIcon(hwnd))
            return -1;
        return 0;

    case WM_DESTROY:
        DeleteNotificationIcon(hwnd);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_RESET:
            break;
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        }
        break;

    case WMAPP_NOTIFYCALLBACK:
        if (wParam == NOTIFICATIONICONID)
        {
            switch (lParam)
            {
            case WM_RBUTTONUP:
                ShowContextMenu(hwnd);
                return 0;
            }
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LPCTSTR lpszClassName = TEXT("MouseBug Window Class");
    LPCTSTR lpszWindowName = TEXT("MouseBug");

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

    return (int)msg.wParam;
}

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Shell32.lib")
