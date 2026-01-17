@echo off
setlocal
title OpenGL Auto-Builder & Runner

:: 1. Configure the project (Generates build files if they don't exist)
echo [1/3] Configuring project with CMake...
cmake -B build -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b %errorlevel%
)

:: 2. Build the project (Generates the .exe for the active lesson in CMakeLists.txt)
echo [2/3] Building active lesson...
cmake --build build --config Debug
if %errorlevel% neq 0 (
    echo [ERROR] Build process failed.
    pause
    exit /b %errorlevel%
)

:: 3. Execute with the correct path context
echo [3/3] Starting application...
echo -----------------------------------------------
:: Move to the directory so that the ../../src/... paths in code work
cd /d "%~dp0build\Debug"

if exist "OpenGL-learn.exe" (
    "OpenGL-learn.exe"
) else (
    echo [ERROR] Generated executable not found.
    pause
)

endlocal