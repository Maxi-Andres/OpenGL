@echo off
setlocal
title OpenGL Learning Project - Installer
cd /d "%~dp0"

echo ============================================================
echo   OPENGL LEARNING PROJECT - INSTALLER
echo ============================================================
echo   GLFW, GLAD, GLM, ImGui and stb_image are already included
echo   in the repo. This installs the two build tools they need:
echo     - CMake
echo     - Visual Studio 2022 Build Tools (C++ compiler)
echo ============================================================
echo.

where winget >nul 2>&1
if errorlevel 1 (
    echo [ERROR] winget was not found. Install "App Installer" from the
    echo         Microsoft Store, then run install.bat again.
    pause
    exit /b 1
)

:: --- CMake ---
set "HAS_CMAKE="
where cmake >nul 2>&1 && set "HAS_CMAKE=1"
if exist "%ProgramFiles%\CMake\bin\cmake.exe" set "HAS_CMAKE=1"

if defined HAS_CMAKE (
    echo [1/2] CMake is already installed.
) else (
    echo [1/2] Installing CMake...
    winget install -e --id Kitware.CMake --accept-source-agreements --accept-package-agreements
    if errorlevel 1 goto :FAILED
)
echo.

:: --- C++ compiler (MSVC) ---
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "HAS_MSVC="
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "HAS_MSVC=1"
)

if defined HAS_MSVC (
    echo [2/2] A Visual Studio C++ compiler is already installed.
) else (
    echo [2/2] Installing Visual Studio 2022 Build Tools with the C++ workload.
    echo       This is a large download and can take a while...
    winget install -e --id Microsoft.VisualStudio.2022.BuildTools --accept-source-agreements --accept-package-agreements --override "--wait --passive --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
    if errorlevel 1 goto :FAILED
)

echo.
echo ============================================================
echo   Done. Double-click run.bat to pick a lesson and run it.
echo ============================================================
pause
exit /b 0

:FAILED
echo.
echo [ERROR] The installation failed. Check the messages above and try again.
pause
exit /b 1
