@echo off
echo [DEBUG] Script starting...
setlocal enabledelayedexpansion
title OpenGL Lesson Manager

:: --- SET INITIAL PATHS ---
set "ROOT_DIR=%~dp0"
set "CONFIG_FILE=%ROOT_DIR%active_lesson.cmake"
set "MODE=Debug"
set "SELECTION=1"

:MENU
cd /d "%ROOT_DIR%"
cls
echo ============================================================
echo   OPENGL LESSON SELECTOR [Mode: %MODE%]
echo ============================================================
echo   1]  Hello Window              12] Projection Matrices
echo   2]  Hello Triangle (2.1)      13] Multiple Objects
echo   3]  Hello Rectangle (2.2)     14] Pyramid
echo   4]  Triangle Colors (2.3)     15] Cube
echo   5]  Cherno Red (2.3)          16] Test Framework
echo   6]  Cherno Shaders (3.1)      17] More Cubes
echo   7]  Basic Shaders (3.2)       18] Camera Class
echo   8]  Interpolation (3.3)       19] Camera Optimization
echo   9]  Abstracting (3.5)         20] Crazy Sun (Light)
echo   10] Textures                  21] Orbiting Cubes
echo   11] Maths
echo ------------------------------------------------------------
echo   CURRENT SELECTION: [%SELECTION%]
echo   [M] Toggle Mode  [R] Clean Build  [X] Exit
echo   [ENTER] Build and Run Selected
echo ------------------------------------------------------------

set "input="
set /p input="Enter Choice: "

:: EXIT LOGIC
if /i "%input%"=="x" goto :EXIT_PROGRAM

:: COMMANDS
if /i "%input%"=="m" goto TOGGLE_MODE
if /i "%input%"=="r" goto CLEANUP
if "%input%"=="" goto PREPARE_BUILD

:: UPDATE SELECTION
set "SELECTION=%input%"
goto MENU

:TOGGLE_MODE
if "%MODE%"=="Debug" (set "MODE=Release") else (set "MODE=Debug")
goto MENU

:PREPARE_BUILD
cls
echo [LOG] Preparing Lesson %SELECTION%...

:: MAP SELECTION TO COMMANDS
if "%SELECTION%"=="1"  set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/1-hello-window/*.cpp")"
if "%SELECTION%"=="2"  set "CMD=set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.1-hello-triangle.cpp")"
if "%SELECTION%"=="3"  set "CMD=set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.2-hello-rectangle.cpp")"
if "%SELECTION%"=="4"  set "CMD=set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.3-hello-triangles-colors.cpp")"
if "%SELECTION%"=="5"  set "CMD=set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.3-theCherno/res/2.1-hello-triangle-red.cpp")"
if "%SELECTION%"=="6"  set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.1-cherno/res/*.cpp")"
if "%SELECTION%"=="7"  set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.2-basic-shaders/*.cpp")"
if "%SELECTION%"=="8"  set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.3-interpolation-uniforms/*.cpp")"
if "%SELECTION%"=="9"  set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3.5-abstracting/*.cpp")"
if "%SELECTION%"=="10" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/4-textures/*.cpp")"
if "%SELECTION%"=="11" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.1-maths/*.cpp")"
if "%SELECTION%"=="12" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.2-projection-matrices/*.cpp")"
if "%SELECTION%"=="13" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.3-rendering-multiple-objects/*.cpp")"
if "%SELECTION%"=="14" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.4-pyramid/*.cpp")"
if "%SELECTION%"=="15" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.5-cube/*.cpp")"
if "%SELECTION%"=="16" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.6-test-framework/*.cpp")"
if "%SELECTION%"=="17" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.7-more-cubes/*.cpp")"
if "%SELECTION%"=="18" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/6-camera/6.1-camera-class/*.cpp")"
if "%SELECTION%"=="19" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/6-camera/6.2-optimization/*.cpp")"
if "%SELECTION%"=="20" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/2-lighting/1-colors-and-2-basic-lighting/1.1-crazy-sun/*.cpp")"
if "%SELECTION%"=="21" set "CMD=file(GLOB_RECURSE SOURCE_FILE "src/2-lighting/1-colors-and-2-basic-lighting/1.2-orbiting-cubes/*.cpp")"

:: Write to file
echo # Generated file > "%CONFIG_FILE%"
echo %CMD% >> "%CONFIG_FILE%"

echo [1/3] Configuring CMake...
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 pause & goto MENU

echo [2/3] Building in %MODE%...
cmake --build build --config %MODE%
if %errorlevel% neq 0 pause & goto MENU

echo [3/3] Running Application...
cd /d "%ROOT_DIR%build\%MODE%"
if exist "OpenGL-learn.exe" (
    "OpenGL-learn.exe"
) else (
    echo [ERROR] .exe not found in build/%MODE%
    pause
)
goto MENU

:CLEANUP
cd /d "%ROOT_DIR%"
if exist build rd /s /q build
if exist "%CONFIG_FILE%" del "%CONFIG_FILE%"
echo Done cleaning.
pause
goto MENU

:EXIT_PROGRAM
cls
echo.
echo ========================================
echo   CLOSING PROGRAM...
echo   Happy Coding!
echo ========================================
timeout /t 2 >nul
exit /b