cmake_minimum_required(VERSION 3.22)
project(OpenGL-learn)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ============================================
# ACTIVE LESSON CONFIGURATION
# ============================================
include(${CMAKE_CURRENT_SOURCE_DIR}/active_lesson.cmake)


set(SOURCES
  src/glad.c
)

# Define la ruta a la carpeta de imgui 
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

