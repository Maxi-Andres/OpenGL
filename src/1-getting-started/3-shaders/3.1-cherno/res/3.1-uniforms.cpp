#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) \
  if (!(x))       \
    __debugbreak();

#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}
static bool GLLogCall(const char *function, const char *file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath)
{
  std::ifstream stream(filepath);

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else
    {
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

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
      -0.5f, 0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f};

  unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3};

  // ======================= CREATE WINDOW =======================
  GLFWwindow *window = glfwCreateWindow(800, 800, "MyFirstWindow", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSwapInterval(6); //! Activa Vsync segun el entero que le pongas

  // Load OpenGL function pointers (GLAD)
  gladLoadGL();

  // Tell OpenGL the size of the rendering area
  glViewport(0, 0, 800, 800);

  // ===================== COMPILE SHADERS =====================

  // Compile vertex shader

  // ShaderProgramSource source = ParseShader("src/2.3-theCherno/res/shaders/Basic.shaders");
  ShaderProgramSource source =
      ParseShader("../../src/1-getting-started/3-shaders/3.1-cherno/res/shaders/uniform.shaders");

  const char *vertexShaderSource = source.VertexSource.c_str();
  const char *fragmentShaderSource = source.FragmentSource.c_str();

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

  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO); // VBO: stores the actual vertex data on the GPU
  glGenBuffers(1, &EBO); // VBO: stores the actual vertex data on the GPU

  // ----------- VAO/VBO SETUP (done once) -------------

  //! Bind VAO so all subsequent VBO/EBO state gets stored inside it
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO so we can copy data to it
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Upload CPU vertex data into GPU memory

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind VBO so we can copy data to it
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Tell OpenGL how to interpret vertex data stored in VBO:
  // location 0 → vec3 → floats → tightly packed → offset 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // Enable the vertex attribute at location 0

  // Unbind VBO (optional safety)
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind VAO so we don't accidentally modify it later
  glBindVertexArray(0);

  GLCall(glUseProgram(shaderProgram)); // Activate shader program

  int location = glGetUniformLocation(shaderProgram, "u_Color");
  ASSERT(location != -1); // puede ser por muchos motivos que no este no es necesario si o si romper todo si no esta el shader
  GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

  // ======================= MAIN LOOP =======================

  float r = 0.0f;
  float increment = 0.05f;
  while (!glfwWindowShouldClose(window))
  {
    // Clear the screen with a background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ----------- DRAWING PHASE (every frame) -------------

    GLCall(glUseProgram(shaderProgram)); // Activate shader program
    glBindVertexArray(VAO);              //! Bind VAO so GPU knows where and how the vertex data is defined

    GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.5)
      increment = 0.05f;

    r += increment;

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
