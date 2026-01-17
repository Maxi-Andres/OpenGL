#include "glad/glad.h" // with this the ouput doesn't say that it has linking problems
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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

  glfwSwapInterval(1); // 0 = VSync off, 1 = VSync on
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
    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y)
    // clang-format off
    float vertices[] = {
        -0.5f, 0.5f, 0.0f, 0.8f, 0.3f, 0.3f, 0.0f, 0.0f, // Top-left
        0.5f, 0.5f, 0.0f, 0.2f, 0.4f, 0.3f,1.0f, 0.0f,// Top-right
        0.5f, -0.5f, 0.0f, 0.1f, 0.3f, 0.7f,1.0f, 1.0f,// Bottom-right
        -0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 0.0f, 1.0f,// Bottom-left
    };
    // clang-format on

    // Index buffer for drawing two triangles (a quad) this has to be an unsigned int
    unsigned int indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ======================= VAO / VBO / EBO =======================
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    // Define vertex layout: position (3 floats) + color (3 floats)
    VertexBufferLayout layout;
    layout.Push<float>(3); // Posición (x, y, z)
    layout.Push<float>(3); // Color (r, g, b)
    layout.Push<float>(2); // texture (x, y)

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= SHADER SETUP =======================

    Shader shader("../../src/1-getting-started/4-textures/shaders/Shaders.shaders");
    shader.Bind(); // this has to match SetUniform1i (default 0)
    shader.SetUniform1i("u_Texture", 0);

    Texture texture("../../res/textures/wall.png");
    texture.Bind();

    //! Unbind everything (good practice to avoid accidental modifications)
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    // ======================= RENDER LOOP =======================

    float r = 0.0f;
    float increment = 0.005f;

    while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);

      renderer.Clear();

      updateTitle(window, "MyFirstWindow"); //* This is only for the fps

      // Activate shader and set uniform
      shader.Bind(); // Get uniform location (must be done after shader program is active)
      shader.SetUniform1f("u_Scale", r);

      renderer.Draw(va, ib, shader);

      // Animate the uniform value
      if (r > 1.5f)
        increment = -0.005f;
      else if (r < 0.0f)
        increment = 0.005f;

      r += increment;

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  } // there is no need to delete anything because of the scope
  glfwTerminate();

  return 0;
}
