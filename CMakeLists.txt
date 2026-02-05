cmake_minimum_required(VERSION 3.22)
project(OpenGL-learn)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============= 1. Getting Started =============

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/1-hello-window/*.cpp")

# set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.1-hello-triangle.cpp")
# set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.2-hello-rectangle.cpp")
# set(SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.3-hello-triangles-colors.cpp")
# set( SOURCE_FILE "src/1-getting-started/2-hello-triangle/2.3-theCherno/res/2.1-hello-triangle-red.cpp")

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.1-cherno/res/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.2-basic-shaders/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3-shaders/3.3-interpolation-uniforms/*.cpp")

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/3.5-abstracting/*.cpp")

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/4-textures/*.cpp")

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.1-maths/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.2-projection-matrices/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.3-rendering-multiple-objects/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.4-pyramid/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.5-cube/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.6-test-framework/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/5.7-more-cubes/*.cpp")

# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/6-camera/6.1-camera-class/*.cpp")
# file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/6-camera/6.2-optimization/*.cpp")

# ============= 2. Lighting =============

# file(GLOB_RECURSE SOURCE_FILE "src/2-lighting/1-colors-and-2-basic-lighting/1.1-crazy-sun/*.cpp")
file(GLOB_RECURSE SOURCE_FILE "src/2-lighting/1-colors-and-2-basic-lighting/1.2-orbiting-cubes/*.cpp")


set(SOURCES
  src/glad.c
)

# Define la ruta a la carpeta de imgui (ajusta si es necesario)
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/vendor/imgui)

# Lista todos los archivos fuente necesarios de ImGui
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

# Agrega todos estos archivos a tu ejecutable
add_executable(OpenGL-learn 
    ${SOURCE_FILE} 
    ${SOURCES} 
    ${IMGUI_SOURCES}
)

target_include_directories(OpenGL-learn PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  # ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
target_link_libraries(OpenGL-learn ${GLFW3_LIBRARY} OpenGL::GL)

# target_link_libraries(OpenGL-learn PRIVATE ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
# target_compile_definitions(OpenGL-learn PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

