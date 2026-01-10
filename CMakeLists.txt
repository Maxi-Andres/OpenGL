cmake_minimum_required(VERSION 3.22)
project(OpenGL-learn)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# set(SOURCE_FILE 
#   src/1-getting-started/2.3-hello-triangles-colors.cpp)

set(SOURCE_FILE 
  src/1-getting-started/3-shaders/3.1-cherno/res/3.1-uniforms.cpp)


set(SOURCES
  src/glad.c
)

add_executable(OpenGL-learn
  ${SOURCE_FILE}
  ${SOURCES}
)


target_include_directories(OpenGL-learn PRIVATE include lib shaders)

target_link_libraries(OpenGL-learn ${GLFW3_LIBRARY} OpenGL::GL)

# target_link_libraries(OpenGL-learn PRIVATE ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
# target_compile_definitions(OpenGL-learn PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

