@ echo off
  setlocal enableextensions enabledelayedexpansion

  rem This batch file builds a program using gcc.
  rem
  rem Copyright (c) 2021 Jorge Ramos (mailto jramos at pobox dot com)
  rem
  rem This is free software. Redistribution and use in source and binary forms,
  rem with or without modification, for any purpose and with or without fee are
  rem hereby permitted. Altered source versions must be plainly marked as such.
  rem
  rem If you find this software useful, an acknowledgment would be appreciated
  rem but is not required.
  rem
  rem THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
  rem IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
  rem FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

  rem https://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.html

:start
  set DEBUG=0
  set CONSOLEAPP=
  set WINDOWSAPP=
  set WINDOWSDLL=
  set MSYS2ROOT=%~1
  set PLATFORM=x86
  set CONFIG=Debug
  set OBJDIR=
  set EXEDIR=
  set EXEFILE=
  set CFLAGS=
  set CXXFLAGS=
  set GCCFLAGS=-mwin32
  set LIBS=

  if "%DEBUG%" == "1" echo MSYS2ROOT %MSYS2ROOT%

  shift
  set PARAM=%~1
  if "%PARAM%" == "" goto displayHelp

  set PREVPARAM1=
  set PREVPARAM2=

:parseLoop
  if "%DEBUG%" == "1" echo PARAM %PARAM%

:checkPREVPARAM1
  if "%PREVPARAM1%" == "-o" (
    set EXEFILE=%PARAM%
    goto parseNext
  )

  if "%PREVPARAM1%" == "/cflags" (
    set CFLAGS=%PARAM%
    goto parseNext
  )

  if "%PREVPARAM1%" == "/cxxflags" (
    set CXXFLAGS=%PARAM%
    goto parseNext
  )

  if "%PREVPARAM1%" == "-std" (
    if "%PREVPARAM2%" == "/cflags" (
      set "CFLAGS=%CFLAGS%=%PARAM%"
      goto parseNext
    )
    if "%PREVPARAM2%" == "/cxxflags" (
      set "CXXFLAGS=%CXXFLAGS%=%PARAM%"
      goto parseNext
    )
    set "GCCFLAGS=%GCCFLAGS%=%PARAM%"
    goto parseNext
  )

  if "%PARAM%" == "-o" goto parseNext
  if "%PARAM%" == "/cflags" goto parseNext
  if "%PARAM%" == "/cxxflags" goto parseNext
  if "%PARAM%" == "-std" goto parseNext

:checkParam
  if /i "%PARAM%" == "/console" (
    set GCCFLAGS=%GCCFLAGS% -mconsole
    set CONSOLEAPP=1
    goto parseNext
  )

  if /i "%PARAM%" == "/windows" (
    set GCCFLAGS=%GCCFLAGS% -mwindows
    set WINDOWSAPP=1
    goto parseNext
  )

  if /i "%PARAM%" == "/dll" (
    set GCCFLAGS=%GCCFLAGS% -mdll
    set WINDOWSDLL=1
    goto parseNext
  )

  if /i "%PARAM%" == "/x86-debug" (
    set PLATFORM=x86
    set CONFIG=Debug
    goto parseNext
  )

  if /i "%PARAM%" == "/x86-release" (
    set PLATFORM=x86
    set CONFIG=Release
    goto parseNext
  )

  if /i "%PARAM%" == "/x64-debug" (
    set PLATFORM=x64
    set CONFIG=Debug
    goto parseNext
  )

  if /i "%PARAM%" == "/x64-release" (
    set PLATFORM=x64
    set CONFIG=Release
    goto parseNext
  )

  if /i "%PARAM%" == "-help" goto displayHelp
  if /i "%PARAM%" == "/help" goto displayHelp

  set GCCFLAGS=%GCCFLAGS% %PARAM%

:parseNext
  shift
  set PREVPARAM2=%PREVPARAM1%
  set PREVPARAM1=%PARAM%
  set PARAM=%~1
  if not "%PARAM%" == "" goto parseLoop

:setPath
  set BINDIR=

  if "%PLATFORM%" == "x86" set BINDIR=%MSYS2ROOT%/mingw32/bin
  if "%PLATFORM%" == "x64" set BINDIR=%MSYS2ROOT%/mingw64/bin

  if "%DEBUG%" == "1" echo BINDIR %BINDIR%

  if "%BINDIR%" == "" (
    echo mingw bin directory not found
    exit /b 3
  )

  if not exist "%BINDIR%/gcc.exe" (
    echo gcc not found
    exit /b 3
  )

  set PATH=%BINDIR%;%PATH%

:makeOutputDirectories
  if "%EXEDIR%" == "" call :getDirName EXEDIR "%EXEFILE%"

  call :removeTrailingSeparator OBJDIR "%OBJDIR%"
  call :removeTrailingSeparator EXEDIR "%EXEDIR%"

  if not "%OBJDIR%" == "" if not exist "%OBJDIR%/" mkdir "%OBJDIR%" 2>&1
  if not "%EXEDIR%" == "" if not exist "%EXEDIR%/" mkdir "%EXEDIR%" 2>&1

:rungcc
  set CFILES=
  for %%A in (*.c) do (
    set CFILES=!CFILES! %%A
  )

  if "%CFILES%" == "" goto rungpp

  if "%DEBUG%" == "1" echo gcc %GCCFLAGS% %CFLAGS% -o %EXEFILE%%CFILES% %LIBS%
  gcc %GCCFLAGS% %CFLAGS% -o %EXEFILE%%CFILES% %LIBS% 2>&1
  if not "%ERRORLEVEL%" == "0" exit /b 3

:rungpp
  set CXXFILES=
  for %%A in (*.cpp) do (
    set CXXFILES=!CXXFILES! %%A
  )

  if "%CXXFILES%" == "" goto exitProgram

  if not "%CFILES%" == "" (
    echo Cannot mix c and cpp source files
    exit /b 3
  )

  if "%DEBUG%" == "1" echo gcc %GCCFLAGS% %CXXFLAGS% -o %EXEFILE%%CXXFILES% %LIBS%
  gcc %GCCFLAGS% %CXXFLAGS% -o %EXEFILE%%CXXFILES% %LIBS% 2>&1
  if not "%ERRORLEVEL%" == "0" exit /b 3

:exitProgram
  exit /b 0

:getDirName
  if not "%~2" == "" set %1=%~dp2
  goto :eof

:removeTrailingSeparator
  set TEMP=%~2
  if not "%TEMP%" == "" (
    if "%TEMP:~-1%" == "/" (
      set %1=%TEMP:~0,-1%
    ) else (
      if "%TEMP:~-1%" == "\" (
        set %1=%TEMP:~0,-1%
      ) else (
        set %1=%TEMP%
      )
    )
  )
  goto :eof

:displayHelp
  echo Syntax: vsmsys2.cmd msys2_root_path options
  echo Build configuration options (for the generated binaries)
  echo   /X86-debug
  echo   /X86-release
  echo   /x64-debug
  echo   /x64-release
  echo Output type
  echo   /console
  echo   /windows
  echo   /dll
  echo Compiler flags
  echo   /cflags   value (c compiler flags)
  echo   /cxxflags value (c++ compiler flags)
  echo General options
  echo   /help - prints this help message
  echo Any other option is passed to gcc
  exit /b 1
