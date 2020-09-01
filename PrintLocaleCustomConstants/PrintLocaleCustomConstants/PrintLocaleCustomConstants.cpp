#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

// https://docs.microsoft.com/en-us/windows/win32/intl/locale-custom-constants

#define PrintHex(x) _tprintf(_T(#x " = 0x%08X\n"), x)

int _tmain(int argc, LPTSTR argv[])
{
    _CRT_UNUSED(argc);
    _CRT_UNUSED(argv);

    PrintHex(LOCALE_CUSTOM_DEFAULT);
    PrintHex(LOCALE_CUSTOM_UI_DEFAULT);
    PrintHex(LOCALE_CUSTOM_UNSPECIFIED);

    return EXIT_SUCCESS;
}
