@ echo off

  if "%1" == "/x86" goto x86
  if "%1" == "/amd64" goto amd64

  set ARCH=x86
  goto copyParams

:x86
  set ARCH=x86
  goto remove1

:amd64
  set ARCH=amd64
  goto remove1

:remove1
  shift

:copyParams
  set PARAMS=%1

:nextParam
  shift
  if "%1" == "" goto runcl
  set PARAMS=%PARAMS% %1
  goto nextParam

:runcl
echo on
  call vsvars.bat -no_logo -arch=%ARCH% 2>&1
echo on
  cl %PARAMS% 2>&1