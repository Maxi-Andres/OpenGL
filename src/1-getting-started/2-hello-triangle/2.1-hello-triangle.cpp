#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= SHADERS (GPU Code) =======================

// Vertex Shader source (processes each vertex)
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment Shader source (sets the pixel color)
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";

// ====================================================================

int main()
{
  // Initialize GLFW (window + OpenGL context manager)
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // ======================= TRIANGLE VERTICES =======================
  // 3 vertices in normalized device coordinates (-1 to 1)
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f, -0.5f, 0.0f,  // right
      0.0f, 0.5f, 0.0f    // top
  };

  // ======================= CREATE WINDOW =======================
  GLFWwindow *window = glfwCreateWindow(800, 800, "MyFirstWindow", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Load OpenGL function pointers (GLAD)
  gladLoadGL();

  // Tell OpenGL the size of the rendering area
  glViewport(0, 0, 800, 800);

  // ===================== COMPILE SHADERS =====================

  // Compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Link shaders into a single shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Delete shader objects (they're linked now, no longer needed)
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // ===================== VAO + VBO =====================

  GLuint VAO, VBO;

  glGenVertexArrays(1, &VAO); // VAO: stores HOW to read the vertex data
  glGenBuffers(1, &VBO);      // VBO: stores the actual vertex data on the GPU

  // ----------- VAO/VBO SETUP (done once) -------------
  glBindVertexArray(VAO); //! Bind VAO so all settings below get recorded into it

  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO so we can copy data to it
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Upload CPU vertex data into GPU memory

  // Tell OpenGL how to interpret vertex data stored in VBO:
  // location 0 → vec3 → floats → tightly packed → offset 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0

  // Unbind VBO (optional safety)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind VAO so we don't accidentally modify it later
  glBindVertexArray(0);

  // ======================= MAIN LOOP =======================

  while (!glfwWindowShouldClose(window))
  {
    // Clear the screen with a background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ----------- DRAWING PHASE (every frame) -------------
    glUseProgram(shaderProgram);      // Activate shader program
    glBindVertexArray(VAO);           //! Bind VAO so GPU knows where and how the vertex data is defined
    glDrawArrays(GL_TRIANGLES, 0, 3); // Draw 3 vertices as one triangle

    // Swap frame buffers and handle input/events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // ======================= CLEANUP =======================

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
