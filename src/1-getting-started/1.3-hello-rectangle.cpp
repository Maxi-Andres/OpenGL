#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ======================= SHADERS (GPU Code) =======================

// Vertex Shader source (runs once per vertex)
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   // Pass vertex position to clip space (NDC conversion happens later)\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment Shader source (runs once per pixel)
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   // Output a constant orange-ish color for every pixel\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";

// ====================================================================

int main()
{
  // Initialize GLFW: creates window, context, and manages inputs
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Request OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // ======================= CREATE WINDOW =======================
  GLFWwindow *window = glfwCreateWindow(800, 800, "MyFirstWindow", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Make this window's OpenGL context current
  glfwMakeContextCurrent(window);

  // Load OpenGL function addresses (required because OpenGL drivers are dynamic)
  gladLoadGL();

  // Set rendering area (Viewport) to cover the whole window
  glViewport(0, 0, 800, 800);

  // ===================== COMPILE SHADERS =====================

  // Create and compile vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Create and compile fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Create a shader program to combine vertex + fragment shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Individual shader objects are no longer needed after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // ======================= TRIANGLE VERTICES =======================
  // Vertex coordinates in Normalized Device Coordinates (-1 to +1)
  float vertices[] = {
      -0.5f, 0.5f, 0.0f, // left top
      0.5f, -0.5f, 0.0f, // right bottom
      0.5f, 0.5f, 0.0f,  // right top
      -0.5f, -0.5f, 0.0f // left bottom
  };

  // Index buffer that tells OpenGL which vertices build triangles
  unsigned int indices[] = {
      0, 1, 2, // top right triangle
      0, 1, 3  // bottom left triangle
  };

  // ===================== VAO + VBO + EBO =====================

  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO); // VAO (Vertex Array Object) stores vertex attribute configuration (how to read vertex data)

  glGenBuffers(1, &VBO); // VBO (Vertex Buffer Object) stores vertex data (positions) on the GPU

  glGenBuffers(1, &EBO); // EBO (Element Buffer Object) stores index data so vertices can be reused

  // Bind VAO so all subsequent VBO/EBO state gets stored inside it
  glBindVertexArray(VAO);

  // Bind VBO and upload vertex array into GPU memory
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind EBO and upload triangle index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Tell OpenGL how to interpret vertex data inside VBO:
  // index=0 → location of 'aPos'
  // 3 floats per vertex
  // not normalized
  // stride = 3 floats * 4 bytes
  // offset = 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Optional safety unbinds (EBO stays bound to VAO!)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // ======================= MAIN LOOP =======================
  while (!glfwWindowShouldClose(window))
  {
    // Clear the screen to a dark teal color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Activate shader program and VAO to draw our geometry
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Draw using indices from EBO (2 triangles = 6 indices)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap the displayed buffer with the rendered one
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // ======================= CLEANUP =======================

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
