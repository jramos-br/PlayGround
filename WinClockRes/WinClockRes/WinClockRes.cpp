#define _WIN32_WINNT 0x0501 /* Windows XP */

//#include <Windows.h>
#include <tchar.h>
//#include <stdio.h>
#include <conio.h>

int Kbhit(void)
{
    int ch;

    if (_kbhit())
    {
        ch = _gettch();

        switch (ch)
        {
        case 0x00:
            ch = (_gettch() & 0x0FFFFFF) | 0xF0000000;
            break;
        case 0xE0:
            ch = (_gettch() & 0x0FFFFFF) | 0xE0000000;
            break;
        default:
            ch &= 0x0FFFFFFF;
            break;
        }
    }
    else
        ch = 0;

    return ch;
}

void Test(void)
{
    static BOOL frequency_initialized;
    static LARGE_INTEGER frequency_old;
    LARGE_INTEGER frequency;
    BOOL changed = FALSE;

    if (QueryPerformanceFrequency(&frequency))
    {
        if (!frequency_initialized)
        {
            frequency_old = frequency;
            frequency_initialized = TRUE;
        }
        else if (frequency.QuadPart != frequency_old.QuadPart)
        {
            frequency_old = frequency;
            changed = TRUE;
        }
    }
    else
    {
        _ftprintf(stderr, _T("QueryPerformanceFrequency error: 0x%08X"), GetLastError());
    }
}

int _tmain(int argc, LPTSTR argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    for (;;)
    {
        int ch;

        while ((ch = Kbhit()) == 0)
        {
            Test();
        }

        _tprintf(_T("%d 0x%08X\n"), ch, ch);

    }

    return EXIT_SUCCESS;
}
