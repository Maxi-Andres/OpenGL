#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
  // Initialize the GLFW library (needed before using any GLFW function)
  glfwInit();

  // Specify the version of OpenGL we want: 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW to use the core profile (modern OpenGL, no deprecated functions)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window (width, height, title, monitor, share context)
  GLFWwindow *window = glfwCreateWindow(800, 800, "MyFirstWindow", NULL, NULL);
  if (window == NULL)
  {
    // If creation fails, show an error and exit
    std::cout << "Failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Make the created window the current OpenGL context
  glfwMakeContextCurrent(window);

  // Load OpenGL functions using GLAD
  gladLoadGL();

  // Set the OpenGL viewport (x, y, width, height)
  // This tells OpenGL the size of the rendering area
  glViewport(0, 0, 800, 800);

  // Set a clear color (R, G, B, A)
  glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
  // Clear the screen using the color we set
  glClear(GL_COLOR_BUFFER_BIT);
  // Swap the back and front buffers to display the cleared color
  glfwSwapBuffers(window);

  // Main while loop
  while (!glfwWindowShouldClose(window))
  {
    // Check and process window/input events
    glfwPollEvents();
  }

  // Destroy the window before ending the program
  glfwDestroyWindow(window);
  // Terminate GLFW and clean up allocated resources
  glfwTerminate();

  return 0;
}
