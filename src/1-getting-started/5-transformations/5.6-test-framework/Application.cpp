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

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

#include "Fps.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
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

  glfwSwapInterval(2); // 0 = VSync off, 1 = VSync on
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
        -50.0f,  50.0f, 0.0f, 0.8f, 0.3f, 0.3f, 0.0f, 0.0f, // Top-left
        50.0f,  50.0f, 0.0f, 0.2f, 0.4f, 0.3f, 1.0f, 0.0f, // Top-right
        50.0f, -50.0f, 0.0f, 0.1f, 0.3f, 0.7f, 1.0f, 1.0f, // Bottom-right
        -50.0f, -50.0f, 0.0f, 0.2f, 0.8f, 0.1f, 0.0f, 1.0f, // Bottom-left
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
    // VertexArray va;
    // VertexBuffer vb(vertices, sizeof(vertices));

    // Define vertex layout: position (3 floats) + color (3 floats)
    // VertexBufferLayout layout;
    // layout.Push<float>(3); // Position (x, y, z)
    // layout.Push<float>(3); // Color (r, g, b)
    // layout.Push<float>(2); // texture (x, y)

    // va.AddBuffer(vb, layout);

    // IndexBuffer ib(indices, 6); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= TRANSFORMATIONS SETUP =======================

    // one to one pixel mapping
    // glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    // Shader shader("../../src/1-getting-started/5-transformations/5.6-test-framework/shaders/Shaders.shaders");
    // shader.Bind(); // this has to match SetUniform1i (default 0)

    // ======================= TEXTURE SETUP =======================

    // Texture texture("../../res/textures/catbop.png");
    // texture.Bind();

    // shader.SetUniform1i("u_Texture", 0);

    //! Unbind everything (good practice to avoid accidental modifications)
    // va.Unbind();
    // shader.Unbind();
    // vb.Unbind();
    // ib.Unbind();

    Renderer renderer;

    // ======================= ImGUI=======================

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // ======================= RENDER LOOP =======================

    glm::vec3 translationA(250, 250, 0); // starts in the middle of the window
    glm::vec3 translationB(550, 550, 0); // starts in the middle of the window

    float r = 1.0f;
    float increment = 0.005f;

    // ======================= test =======================
    // here yo could select a test so that you wont need to select it every time if you are trying something specific
    test::Test *currentTest = nullptr;
    test::TestMenu *testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

    // test::TestClearColor test;

    while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);

      updateTitle(window, "MyFirstWindow"); //* This is only for the fps

      GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); // se podria poner en renderer.h esto
      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      if (currentTest)
      {
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("Test");
        if (currentTest != testMenu && ImGui::Button("<-"))
        {
          delete currentTest;
          currentTest = testMenu;
        }
        currentTest->OnImGuiRender();
        ImGui::End();
      }

      // Activate shader and set uniform
      // shader.Bind(); // Get uniform location (must be done after shader program is active)
      // shader.SetUniform1f("u_Scale", r);

      //! this is not a good way of rendering a lot of things, what you what to do is batch rendering in a single draw call
      // {
      //   glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      //   glm::mat4 mvp = proj * view * model;
      //   shader.SetUniformMat4f("u_MVP", mvp);

      //   renderer.Draw(va, ib, shader);
      // }

      // {
      //   glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
      //   glm::mat4 mvp = proj * view * model;
      //   shader.SetUniformMat4f("u_MVP", mvp);

      //   renderer.Draw(va, ib, shader);
      // }

      // Animate the uniform value
      // if (r > 1.5f)
      //   increment = -0.005f;
      // else if (r < 0.0f)
      //   increment = 0.005f;

      // r += increment;

      {
        // ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 800.0f);
        // ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 800.0f);
        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    delete currentTest;
    if (currentTest != testMenu)
      delete testMenu;

  } // there is no need to delete anything because of the scope

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  return 0;
}
