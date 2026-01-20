# OpenGL Learning Project

A progressive OpenGL 3.3 Core Profile learning project following the [LearnOpenGL](https://learnopengl.com) tutorial series.

## Overview

This is a structured learning repository based on the excellent LearnOpenGL tutorial by Joey de Vries. Each folder represents a complete lesson in OpenGL development. The project uses CMake to easily switch between different lessons by modifying the source files in `CMakeLists.txt`.

![5.5-cube](/res/gifs/2026-01-19-19-40-45.gif)
(**5.5-cube Demo**)

## Features

- **Progressive learning structure** - organized by topics and difficulty
- **OpenGL 3.3 Core Profile** - modern OpenGL practices
- **Based on [LearnOpenGL.com](https://learnopengl.com)** - industry-standard learning resource
- **Modular architecture** - each lesson is self-contained
- **Easy lesson switching** - configure via CMakeLists.txt
- **Work in progress** - continuously adding new lessons and concepts

## Prerequisites

- CMake 3.22 or higher
- C++11 compatible compiler
- OpenGL 3.3+ compatible graphics driver
- GLFW3 library
- GLAD (included in project)

## Building the Project

### Select Your Lesson

Edit `CMakeLists.txt` and set the `SOURCE_FILE` variable to the lesson you want to run:

```cmake
# Example: Single-file lesson
set(SOURCE_FILE
  src/1-getting-started/2.1-hello-triangle.cpp)

# Example: Multi-file lesson with abstractions
file(GLOB SOURCE_FILE "src/1-getting-started/3.5-abstracting/*.cpp")
```

### Run Program (Windows)

To quickly compile and run the selected lesson without opening an IDE:

- Save your changes in CMakeLists.txt.

- Double-click compile_and_run.bat in the root folder.

The script will automatically configure the project, build the latest code, and launch the executable using the correct working directory to ensure Shaders load properly.

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
