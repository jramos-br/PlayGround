#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>

void ErrorExit(LPCTSTR lpszMessage)
{
    _ftprintf(stderr, _T("%s\n"), lpszMessage);
    exit(EXIT_FAILURE);
}

int _tmain(int argc, _TCHAR* argv[])
{
    _CRT_UNUSED(argc);
    _CRT_UNUSED(argv);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
        ErrorExit(_T("GetStdHandle"));

    return EXIT_SUCCESS;
}
