cmake_minimum_required(VERSION 3.22)
project(OpenGL-learn)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(SOURCE_FILE 
#   src/1-getting-started/2.3-hello-triangles-colors.cpp)

  file(GLOB_RECURSE SOURCE_FILE "src/1-getting-started/5-transformations/*.cpp")

set(SOURCES
  src/glad.c
)

add_executable(OpenGL-learn
  ${SOURCE_FILE}
  ${SOURCES}
)

target_include_directories(OpenGL-learn PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)
target_link_libraries(OpenGL-learn ${GLFW3_LIBRARY} OpenGL::GL)

# target_link_libraries(OpenGL-learn PRIVATE ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
# target_compile_definitions(OpenGL-learn PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

