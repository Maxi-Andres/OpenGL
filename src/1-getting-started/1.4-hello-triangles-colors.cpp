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
const char *fragmentShaderSource1 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   // Output a constant orange-ish color for every pixel\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";

const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   // Output a constant orange-ish color for every pixel\n"
    "   FragColor = vec4(0.3f, 0.8f, 0.02f, 1.0f);\n"
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

  // red
  GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);

  // green
  GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  GLuint shaderProgram1 = glCreateProgram();
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  GLuint shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  // Individual shader objects are no longer needed after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  // ======================= TRIANGLE VERTICES =======================
  // Vertex coordinates in Normalized Device Coordinates (-1 to +1)
  float firstTriangle[] = {
      -0.9f, -0.5f, 0.0f, // left
      -0.0f, -0.5f, 0.0f, // right
      -0.45f, 0.5f, 0.0f  // top
  };
  float secondTriangle[] = {
      0.0f, -0.5f, 0.0f, // left
      0.9f, -0.5f, 0.0f, // right
      0.45f, 0.5f, 0.0f  // top
  };

  // ===================== VAO + VBO  =====================

  GLuint VAOs[2], VBOs[2];

  glGenVertexArrays(2, VAOs); // VAO (Vertex Array Object) stores vertex attribute configuration (how to read vertex data)
  glGenBuffers(2, VBOs);      // VBO (Vertex Buffer Object) stores vertex data (positions) on the GPU

  // first triangle
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines

  // second triangle
  glBindVertexArray(VAOs[1]);             // note that we bind to a different VAO now
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]); // and a different VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
  glEnableVertexAttribArray(0);
  // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)

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
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap the displayed buffer with the rendered one
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // ======================= CLEANUP =======================

  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  glDeleteProgram(shaderProgram1);
  glDeleteProgram(shaderProgram2);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
