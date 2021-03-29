@ echo off
  setlocal

  rem This batch file locates and runs cl.exe, which is a tool that controls the
  rem Microsoft C and C++ compilers and linker.
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

  rem https://github.com/microsoft/vswhere
  rem https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line

  set TARGET_ARCH=x86
  set HOST_ARCH=x86
  set CONFIG=Debug
  set OBJDIR=
  set BINDIR=
  set EXEPATH=
  set CLOPTS=/nologo

  set PARAM=%~1
  if "%PARAM%" == "" goto displayHelp

:parseLoop
  if /i "%PARAM%" == "-help" goto displayHelp
  if /i "%PARAM%" == "/help" goto displayHelp

  if /i "%PARAM%" == "/win32-debug" goto win32Debug
  if /i "%PARAM%" == "/win32-release" goto win32Release
  if /i "%PARAM%" == "/x64-debug" goto x64Debug
  if /i "%PARAM%" == "/x64-release" goto x64Release
  if /i "%PARAM%" == "/arm-debug" goto armDebug
  if /i "%PARAM%" == "/arm-release" goto armRelease
  if /i "%PARAM%" == "/arm64-debug" goto arm64Debug
  if /i "%PARAM%" == "/arm64-release" goto arm64Release

  if /i "%PARAM%" == "/ha-x86" goto x86HostArch
  if /i "%PARAM%" == "/ha-x64" goto x64HostArch

  if "%PARAM:~0,3%" == "/Fo" set OBJDIR=%PARAM:~3%
  if "%PARAM:~0,3%" == "/Fe" set EXEPATH=%PARAM:~3%

  set CLOPTS=%CLOPTS% %PARAM%
  goto parseNext

:displayHelp
  echo Syntax: vsruncl.bat [options]
  echo Build configuration options (for the generated binaries)
  echo   /win32-debug
  echo   /win32-release
  echo   /x64-debug
  echo   /x64-release
  echo   /arm-debug
  echo   /arm-release
  echo   /arm64-debug
  echo   /arm64-release
  echo Host architecture options (compiler binaries to be used)
  echo   /ha-x86
  echo   /ha-x64
  echo General options
  echo   /help - prints this help message
  echo Any other option is passed to cl.exe
  exit /b 1

:win32Debug
  set TARGET_ARCH=x86
  set CONFIG=Debug
  goto parseNext

:win32Release
  set TARGET_ARCH=x86
  set CONFIG=Release
  goto parseNext

:x64Debug
  set TARGET_ARCH=x64
  set CONFIG=Debug
  goto parseNext

:x64Release
  set TARGET_ARCH=x64
  set CONFIG=Release
  goto parseNext

:armDebug
  set TARGET_ARCH=arm
  set CONFIG=Debug
  goto parseNext

:armRelease
  set TARGET_ARCH=arm
  set CONFIG=Release
  goto parseNext

:arm64Debug
  set TARGET_ARCH=arm64
  set CONFIG=Debug
  goto parseNext

:arm64Release
  set TARGET_ARCH=arm64
  set CONFIG=Release
  goto parseNext

:x86HostArch
  set HOST_ARCH=x86
  goto parseNext

:x64HostArch
  set HOST_ARCH=x64
  goto parseNext

:parseNext
  shift
  set PARAM=%~1
  if not "%PARAM%" == "" goto parseLoop

:findBuildTools
  set vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
  if exist "%vswhere%" goto vswhereExists
  set vswhere=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe
  if exist "%vswhere%" goto vswhereExists
  echo vswhere not found
  exit /b 3

:vswhereExists
  set INSTALLDIR=
  for /f "usebackq tokens=*" %%i in (`"%vswhere%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set INSTALLDIR=%%i

  if "%INSTALLDIR%" == "" (
    echo Microsoft.VisualStudio.Component.VC.Tools.x86.x64 not found
    exit /b 3
  )

  set VSDEVCMD=%INSTALLDIR%\Common7\Tools\vsdevcmd.bat

  if not exist "%VSDEVCMD%" (
    echo vsdevcmd not found
    exit /b 3
  )

:runVsDevCmd
  set VSCMD_START_DIR="%CD%"
  call "%VSDEVCMD%" -no_logo -arch=%TARGET_ARCH% -host_arch=%HOST_ARCH% 2>&1
  if not "%ERRORLEVEL%" == "0" exit /b 3

:makeOutputDirectories
  if "%BINDIR%" == "" call :getDirName BINDIR "%EXEPATH%"

  call :removeTrailingSeparator OBJDIR "%OBJDIR%"
  call :removeTrailingSeparator BINDIR "%BINDIR%"

  if not "%OBJDIR%" == "" if not exist "%OBJDIR%/" mkdir "%OBJDIR%" 2>&1
  if not "%BINDIR%" == "" if not exist "%BINDIR%/" mkdir "%BINDIR%" 2>&1

:runCl
  cl %CLOPTS% 2>&1
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
