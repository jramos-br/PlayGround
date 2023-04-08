@echo off

rem https://github.com/microsoft/vswhere

set vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
if exist "%vswhere%" goto vswhereExists

set vswhere=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe
if exist "%vswhere%" goto vswhereExists

echo vswhere not found
exit /b 3

:vswhereExists

set InstallDir=
for /f "usebackq tokens=*" %%i in (`"%vswhere%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set InstallDir=%%i

if "%InstallDir%" == "" (
  echo Microsoft.VisualStudio.Component.VC.Tools.x86.x64 not found
  exit /b 3
)

set vsdevcmd=%InstallDir%\Common7\Tools\vsdevcmd.bat

if not exist "%vsdevcmd%" (
  echo vsdevcmd not found
  exit /b 3
)

call "%vsdevcmd%" %*