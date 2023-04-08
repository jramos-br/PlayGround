#include <Windows.h>
#include <strsafe.h>
#include <tchar.h>

LPTSTR FormatWin32Message(DWORD dwMessageId, LPTSTR lpszBuffer, size_t cchBuffer)
{
    static const TCHAR szDefaultWin32Message[] = TEXT("Win32 Message ID 0x%08I32X (%I32d)");
    static const DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

    LPTSTR lpszMessage = NULL;
    DWORD dwCount = FormatMessage(dwFlags, NULL, dwMessageId, 0, (LPTSTR)&lpszMessage, 0, NULL);

    if (lpszMessage != NULL)
    {
        if (dwCount > 0)
        {
            while (!_istprint((TBYTE)lpszMessage[dwCount - 1]))
            {
                lpszMessage[--dwCount] = _T('\0'); if (dwCount == 0) break;
            }

            _sntprintf_s(lpszBuffer, cchBuffer, _TRUNCATE, _T("%s (0x%08I32X, %I32d)"), lpszMessage, dwMessageId, dwMessageId);
        }
        else
            _sntprintf_s(lpszBuffer, cchBuffer, _TRUNCATE, szDefaultWin32Message, dwMessageId, dwMessageId);

        LocalFree(lpszMessage);
    }
    else
        _sntprintf_s(lpszBuffer, cchBuffer, _TRUNCATE, szDefaultWin32Message, dwMessageId, dwMessageId);

    return lpszBuffer;
}
