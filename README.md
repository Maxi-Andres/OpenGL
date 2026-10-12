# OpenGL Learning Project

A progressive OpenGL 3.3 Core Profile learning project following the [LearnOpenGL](https://learnopengl.com) tutorial series.

## Overview

This is a structured learning repository based on the excellent LearnOpenGL tutorial by Joey de Vries. Each folder represents a complete lesson in OpenGL development. A small menu (`run.bat`) lets you pick any lesson, then builds and runs it.

![5.5-cube](/res/gifs/2026-01-19-19-40-45.gif)
(**5.5-cube Demo**)

## Features

- **Progressive learning structure** - organized by topics and difficulty
- **OpenGL 3.3 Core Profile** - modern OpenGL practices
- **Based on [LearnOpenGL.com](https://learnopengl.com)** - industry-standard learning resource
- **Modular architecture** - each lesson is self-contained
- **Easy lesson switching** - pick a lesson from the `run.bat` menu
- **Work in progress** - continuously adding new lessons and concepts

## Quick Start (Windows)

1. Double-click **`install.bat`**. It installs CMake and the Visual Studio 2022 C++ Build Tools with `winget`, and skips anything you already have.
2. Double-click **`run.bat`**, pick a lesson from the menu and press **Enter**. It configures CMake, builds the lesson and launches it from the right working directory so shaders and textures load.

Menu options: **M** toggles Debug/Release, **R** cleans the build, **X** exits.

GLFW, GLAD, GLM, ImGui and stb_image are already included in the repo, so there's nothing else to download. You only need a graphics driver that supports OpenGL 3.3 or newer.

## Building Manually

Requirements: CMake 3.22+, a C++11 compiler (MSVC on Windows, since the bundled `glfw3.lib` is built for it) and an OpenGL 3.3+ driver.

Set the lesson in `active_lesson.cmake`, for example:

```cmake
file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.5-cube/*.cpp")
```

Then build and run from `build/Debug`, because lessons load their shaders with paths relative to it:

```bat
cmake -S . -B build -A x64
cmake --build build --config Debug
cd build\Debug && OpenGL-learn.exe
```

### Run Program (Linux)

#### _!TODO_

## Common Controls

Most lessons include:

- **ESC** - Exit application
- Window resizing support (viewport adjusts automatically)

Additional controls are shown within each specific lesson.

## Performance

- VSync can be toggled via `glfwSwapInterval(0)` on each individual Application for unlimited FPS or `glfwSwapInterval(1)` for VSync

## Project Status

🚧 **Work in Progress** - This project is actively being developed as I progress through the LearnOpenGL tutorial series. New lessons and concepts are added regularly.

## Resources

- **Tutorial Website**: [LearnOpenGL.com](https://learnopengl.com)
- **GLFW Documentation**: [glfw.org](https://www.glfw.org/)
- **OpenGL Reference**: [docs.gl](https://docs.gl/)
- **GLAD Generator**: [glad.dav1d.de](https://glad.dav1d.de/)
- **OpenGL Mathematics**: [OpenGL Mathematics](https://glm.g-truc.net/0.9.9/index.html/)
- **ImGUI**: [ImGUI](https://github.com/ocornut/imgui)
- **LEARN C++**: [learncpp](https://www.learncpp.com/)

## Credits

This project follows the structure and lessons from [LearnOpenGL](https://learnopengl.com) by Joey de Vries, licensed under CC BY-NC 4.0.

## License

Educational/Learning Project
