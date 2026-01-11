#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// ======================= ERROR HANDLING =======================

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
    std::cout << "[OpenGL Error] (" << error << "): "
              << function << " | " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

// ======================= SHADER PARSER =======================

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

int main()
{
  // ======================= INIT GLFW =======================
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "MyFirstWindow", NULL, NULL);
  if (!window)
  {
    std::cout << "Failed to create a window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  glfwSwapInterval(9); // VSync dependiento el int

  // ======================= INIT GLAD =======================
  gladLoadGL();
  glViewport(0, 0, 800, 800);

  // ======================= VERTEX DATA =======================
  float vertices[] = {
      -0.5f, 0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f};

  unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3};

  // ======================= SHADER SETUP =======================
  ShaderProgramSource source = ParseShader("../../src/1-getting-started/3-shaders/3.1-cherno/res/shaders/uniform.shaders");
  const char *vs = source.VertexSource.c_str();
  const char *fs = source.FragmentSource.c_str();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vs, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fs, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // ======================= VAO / VBO / EBO =======================
  unsigned int VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // No usamos arrays normales porque la GPU no puede leer RAM.Ponemos todo en buffers para que quede en VRAM y el pipeline lo use solo. por eso se hace esto

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  //! el primer 0 de la firma de esta funcion significa que en el index 0 de este vertex array (VAO) esta bound a el currently bound GL_ARRAY_BUFFER
  //! tambien si te das cuenta es el elemetn array porque usa float (por los vertices) y no unsited int como en indices

  // ======================= UNIFORM LOCATION =======================
  // Importante: glGetUniformLocation solo funciona si hay un shader program activo.
  GLCall(glUseProgram(shaderProgram));
  int uLocation = glGetUniformLocation(shaderProgram, "u_Color");
  ASSERT(uLocation != -1);

  //! siempre se desviendea todo como buena practica
  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // ======================= RENDER LOOP =======================
  float r = 0.0f;
  float increment = 0.05f;

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    GLCall(glUseProgram(shaderProgram));
    GLCall(glUniform4f(uLocation, r, 0.3f, 0.8f, 1.0f));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // probe a no bindear el GL_ELEMENT_ARRAY_BUFFER y funciona igual, esto es porque el VAO guarda info pero me parece qeu en ciertas situaciones puede romper

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
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
}
