#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// ======================= FPS COUNTER FUNCTIONS =======================

#include <iomanip>

float mDeltaTime = 0.0f;
float mLastFrame = 0.0f;
float mPreviousTime = 0.0f;
int mFrameCount = 0;
float mFPS = 0.0f;

void updateTitle(GLFWwindow *window, const std::string &title);

float getFPS()
{
  float currentFrame = glfwGetTime();
  mDeltaTime = currentFrame - mLastFrame;
  mLastFrame = currentFrame;
  mFrameCount++;

  if (currentFrame - mPreviousTime >= 1.0f)
  {
    mFPS = mFrameCount / (currentFrame - mPreviousTime);
    mPreviousTime = currentFrame;
    mFrameCount = 0;
  }

  return mFPS;
}

void updateTitle(GLFWwindow *window, const std::string &title)
{
  float fps = getFPS();
  std::stringstream ss;
  ss << title << " | FPS: " << std::fixed << std::setprecision(2) << fps;
  std::string newTitle = ss.str();
  glfwSetWindowTitle(window, newTitle.c_str());
}
//! ============================================================

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

//! ============================================================

// ======================= SHADER PARSER =======================

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

// Parses a shader file containing both vertex and fragment shaders
// Expects #shader vertex and #shader fragment directives to separate the two
static ShaderProgramSource ParseShader(const std::string &filepath)
{
  std::ifstream stream(filepath);

  if (!stream.is_open())
  {
    std::cout << "[ERROR] Failed to open shader file at: "
              << filepath << std::endl;
    return {};
  }

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

// Compiles an individual shader (vertex or fragment) and checks for GLSL syntax errors
static unsigned int CompileShader(unsigned int type, const std::string &source)
{
  GLCall(unsigned int id = glCreateShader(type));
  const char *src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  // Check for compilation errors
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

// Compiles both vertex and fragment shaders, links them into a GPU program, validates it, and cleans up individual shaders
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  // Delete individual shaders as they're now linked into the program
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

int main()
{
  // ======================= INIT GLFW =======================
  if (!glfwInit())
    return -1;

  // Request OpenGL 3.3 Core Profile
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
  // Make the window's context current
  glfwMakeContextCurrent(window);

  glfwSwapInterval(0); // 0 = VSync off, 1 = VSync on
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ======================= INIT GLAD =======================
  // Load all OpenGL function pointers through GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  //! Scope ensures proper destruction order of OpenGL objects before context is destroyed
  {
    // ======================= VERTEX DATA =======================
    // Interleaved vertex data: position (x, y, z) + color (r, g, b)
    float vertices[] = {
        -0.5f, 0.5f, 0.0f, 0.8f, 0.3f, 0.3f, // Top-left
        0.5f, 0.5f, 0.0f, 0.2f, 0.4f, 0.3f,  // Top-right
        0.5f, -0.5f, 0.0f, 0.1f, 0.3f, 0.7f, // Bottom-right
        -0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f // Bottom-left
    };

    // Index buffer for drawing two triangles (a quad) this has to be an unsigned int
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // ======================= VAO / VBO / EBO =======================
    // unsigned int VAO, VBO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    // Define vertex layout: position (3 floats) + color (3 floats)
    VertexBufferLayout layout;
    layout.Push<float>(3); // Posición (x, y, z)
    layout.Push<float>(3); // Color (r, g, b)

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= SHADER SETUP =======================

    ShaderProgramSource source = ParseShader("../../src/1-getting-started/3-shaders/3.4-abstracting/shaders/Shaders.shaders");

    unsigned int shaderProgram = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shaderProgram));

    // ======================= UNIFORM LOCATION =======================
    // Get uniform location (must be done after shader program is active)
    GLCall(int uLocation = glGetUniformLocation(shaderProgram, "scale"));
    ASSERT(uLocation != -1);

    //! Unbind everything (good practice to avoid accidental modifications)
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    // ======================= RENDER LOOP =======================

    float r = 0.0f;
    float increment = 0.0005f;

    while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);

      GLCall(glClear(GL_COLOR_BUFFER_BIT));

      updateTitle(window, "MyFirstWindow"); //* This is only for the fps

      // Activate shader and set uniform
      GLCall(glUseProgram(shaderProgram));
      GLCall(glUniform1f(uLocation, r));

      // Bind VAO and index buffer, then draw
      va.Bind();
      ib.Bind();

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

      // Animate the uniform value
      if (r > 1.5f)
        increment = -0.0005f;
      else if (r < 0.0f)
        increment = 0.0005f;

      r += increment;

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // ======================= CLEANUP =======================
    // GLCall(glDeleteVertexArrays(1, &VAO));
    // GLCall(glDeleteBuffers(1, &VBO));
    // GLCall(glDeleteBuffers(1, &EBO));
    GLCall(glDeleteProgram(shaderProgram));
  }
  // glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
