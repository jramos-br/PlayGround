// Get the current time from a POSIX clockid_t. Windows version.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

#include "uclock.h"

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <stdint.h>

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#endif

#define MS_PER_SEC      1000            // MS = millisecond
#define US_PER_MS       1000            // US = microsecond
#define NS_PER_US       1000            // NS = nanosecond
#define FT_PER_US       10              // FT = 100ns

#define NS_PER_FT       (NS_PER_US / FT_PER_US)
#define US_PER_SEC      (US_PER_MS * MS_PER_SEC)
#define NS_PER_SEC      (NS_PER_US * US_PER_SEC)
#define FT_PER_SEC      (FT_PER_US * US_PER_SEC)
#define NS_PER_MS       (NS_PER_US * US_PER_MS)
#define FT_PER_MS       (FT_PER_US * US_PER_MS)

// Seconds between the Windows epoch (Jan. 1, 1601) and the Unix epoch (Jan. 1, 1970).
#define UNIX_EPOCH_SEC  INT64_C(11644473600)
#define UNIX_EPOCH_FT   (UNIX_EPOCH_SEC * FT_PER_SEC)

static BOOL GetPerformanceFrequency(LARGE_INTEGER* pFrequency)
{
    static LARGE_INTEGER frequency;

    if (frequency.QuadPart == 0)
    {
        if (!QueryPerformanceFrequency(&frequency) || frequency.QuadPart == 0)
        {
            return 0;
        }
    }

    *pFrequency = frequency;
    return 1;
}

static int GetPerformanceCounter(LARGE_INTEGER* pcounter, LARGE_INTEGER* pfrequency)
{
    static LARGE_INTEGER frequency;

    if (frequency.QuadPart == 0)
    {
        if (!QueryPerformanceFrequency(&frequency) || frequency.QuadPart == 0)
        {
            return -1;
        }
    }

    *pfrequency = frequency;

    if (!QueryPerformanceCounter(pcounter))
    {
        return -1;
    }

    return 0;
}

static int do_realtime(struct timespec* tp)
{
    FILETIME systemTimeAsFileTime;
    ULARGE_INTEGER systemTime;

#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
    GetSystemTimePreciseAsFileTime(&systemTimeAsFileTime);
#else
    GetSystemTimeAsFileTime(&systemTimeAsFileTime);
#endif

    systemTime.LowPart = systemTimeAsFileTime.dwLowDateTime;
    systemTime.HighPart = systemTimeAsFileTime.dwHighDateTime;

    systemTime.QuadPart -= UNIX_EPOCH_FT;

    ULONGLONG q = systemTime.QuadPart / FT_PER_SEC;
    ULONGLONG r = systemTime.QuadPart % FT_PER_SEC;

    tp->tv_sec = q;
    tp->tv_nsec = (long)r * NS_PER_FT;

    return 0;
}

static int do_realtime_coarse(struct timespec* tp)
{
    FILETIME systemTimeAsFileTime;
    ULARGE_INTEGER systemTime;

    GetSystemTimeAsFileTime(&systemTimeAsFileTime);

    systemTime.LowPart = systemTimeAsFileTime.dwLowDateTime;
    systemTime.HighPart = systemTimeAsFileTime.dwHighDateTime;

    systemTime.QuadPart -= UNIX_EPOCH_FT;

    ULONGLONG q = systemTime.QuadPart / FT_PER_SEC;
    ULONGLONG r = systemTime.QuadPart % FT_PER_SEC;

    tp->tv_sec = q;
    tp->tv_nsec = (long)r * NS_PER_FT;

    return 0;
}

static int do_monotonic(struct timespec* tp)
{
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;

    if (GetPerformanceCounter(&counter, &frequency) < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ULONGLONG q = counter.QuadPart / frequency.QuadPart;
    ULONGLONG r = counter.QuadPart % frequency.QuadPart;

    tp->tv_sec = q;
    tp->tv_nsec = (long)((r * NS_PER_SEC + (frequency.QuadPart / 2)) / frequency.QuadPart);

    if (tp->tv_nsec >= NS_PER_SEC)
    {
        tp->tv_sec++;
        tp->tv_nsec -= NS_PER_SEC;
    }

    return 0;
}

static int do_monotonic_coarse(struct timespec* tp)
{
    ULONGLONG count = GetTickCount64();

    tp->tv_sec = count / MS_PER_SEC;
    tp->tv_nsec = count % MS_PER_SEC * NS_PER_MS;

    return 0;
}

int clock_getres(clockid_t clk_id, struct timespec* res)
{
    LARGE_INTEGER frequency;
    DWORD   timeAdjustment, timeIncrement;
    BOOL    isTimeAdjustmentDisabled;

    switch (clk_id)
    {
    case CLOCK_MONOTONIC:

        if (!GetPerformanceFrequency(&frequency))
        {
            errno = EINVAL;
            return -1;
        }

        res->tv_sec = 0;
        res->tv_nsec = (long)((NS_PER_SEC + (frequency.QuadPart / 2)) / frequency.QuadPart);

        if (res->tv_nsec >= NS_PER_SEC)
        {
            res->tv_sec++;
            res->tv_nsec -= NS_PER_SEC;
        }
        else if (res->tv_nsec < 1)
            res->tv_nsec = 1;

        return 0;

    case CLOCK_MONOTONIC_COARSE:

        res->tv_sec = 0;
        res->tv_nsec = NS_PER_MS;
        return 0;

    case CLOCK_REALTIME:
    case CLOCK_REALTIME_COARSE:

        (void)GetSystemTimeAdjustment(&timeAdjustment, &timeIncrement, &isTimeAdjustmentDisabled);
        res->tv_sec = 0;
        res->tv_nsec = timeIncrement * 100;

        return 0;
    }

    errno = EINVAL;
    return -1;
}

int clock_gettime(clockid_t clk_id, struct timespec* tp)
{
    switch (clk_id)
    {
    case CLOCK_REALTIME:
        return do_realtime(tp);
    case CLOCK_MONOTONIC:
        return do_monotonic(tp);
    case CLOCK_REALTIME_COARSE:
        return do_realtime_coarse(tp);
    case CLOCK_MONOTONIC_COARSE:
        return do_monotonic_coarse(tp);
    }

    errno = EINVAL;
    return -1;
}


int clock_settime(clockid_t clk_id, const struct timespec* tp)
{
    return 0;
}

static HMODULE GetNtDll()
{
    static HMODULE hNtDll;

    if (hNtDll == NULL)
    {
        hNtDll = GetModuleHandle(_T("Ntdll.dll"));
    }

    return hNtDll;
}

typedef NTSTATUS(CALLBACK* LPFN_NtQueryTimerResolution)(PULONG, PULONG, PULONG);

NTSTATUS NtQueryTimerResolution(OUT PULONG MinimumResolution, OUT PULONG MaximumResolution, OUT PULONG ActualResolution)
{
    static LPFN_NtQueryTimerResolution pNtQueryTimerResolution;

    if (pNtQueryTimerResolution == NULL)
    {
        HMODULE hNtDll = GetNtDll();

        if (hNtDll == NULL)
        {
            return STATUS_DLL_NOT_FOUND;
        }

        pNtQueryTimerResolution = (LPFN_NtQueryTimerResolution)GetProcAddress(hNtDll, "NtQueryTimerResolution");

        if (pNtQueryTimerResolution == NULL)
        {
            return STATUS_ENTRYPOINT_NOT_FOUND;
        }
    }

    return (*pNtQueryTimerResolution)(MinimumResolution, MaximumResolution, ActualResolution);
}

void clock_test1()
{
    DWORD timeAdjustment;
    DWORD timeIncrement;
    BOOL isTimeAdjustmentDisabled;

    ULONG nMinRes, nMaxRes, nCurRes;
    if (NtQueryTimerResolution(&nMinRes, &nMaxRes, &nCurRes) == STATUS_SUCCESS)
    {
        printf("NT timer resolutions (min/max/cur): %u.%u / %u.%u / %u.%u ms\n",
            nMinRes / 10000, (nMinRes % 10000) / 10,
            nMaxRes / 10000, (nMaxRes % 10000) / 10,
            nCurRes / 10000, (nCurRes % 10000) / 10);
    }
    else
        printf("ERRO\n");

    for (int i = 0; i < 10; ++i)
    {
        timeAdjustment = 0;
        timeIncrement = 0;
        isTimeAdjustmentDisabled = 0;

        if (GetSystemTimeAdjustment(&timeAdjustment, &timeIncrement, &isTimeAdjustmentDisabled))
        {
            printf("timeAdjustment=%lu timeIncrement=%lu isTimeAdjustmentDisabled=%d\n", timeAdjustment, timeIncrement, isTimeAdjustmentDisabled);
        }
        else
        {
            printf("erro\n");
        }

        Sleep(1);
    }
}
