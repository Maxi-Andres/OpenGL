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

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
  // ======================= INIT GLFW =======================
  if (!glfwInit())
    return -1;

  // Request OpenGL 3.3 Core Profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, "MyFirstWindow", NULL, NULL);
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
    //! SI USAS ESTO FUNCIONA EL TEMA ES QUE NO QUEDAN BIEN LAS TEXTURAS PORQUE NO TENES LOS SUFICIENTES PUNTOS
    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y)
    // clang-format off
    // float vertices[] = {
    //     // cube base
    //     -50.0f, -50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 0 (-x , +z is red)
    //     -50.0f, -50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 1 (-x , -z is green)
    //      50.0f, -50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 2 (x , -z is blue)
    //      50.0f, -50.0f,  50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // 3 (x , z is white)
        
    //      // cube top
    //     -50.0f,  50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 4 (-x , +z is red)
    //     -50.0f,  50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 5 (-x , -z is green)
    //      50.0f,  50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 6 (x , -z is blue)
    //      50.0f,  50.0f,  50.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // 7 (x , z is white)
    // };
    // clang-format on

    // Index buffer for drawing two triangles (a quad) this has to be an unsigned int
    // clang-format off
    // unsigned int indices[] = {
    //     // BOTTOM
    //     0, 1, 2,
    //     0, 2, 3,
    //     // TOP
    //     4, 5, 6,
    //     4, 6, 7,
    //     // LEFT
    //     0, 4, 5,
    //     0, 5, 1,
    //     // RIGHT (derecha)
    //     3, 2, 6,
    //     3, 6, 7,
    //     // FRONT
    //     0, 3, 7,
    //     0, 7, 4,
    //     // BACK 
    //     1, 5, 6,
    //     1, 6, 2
    //   };
    // clang-format on

    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y)
    // clang-format off
    float vertices[] = {
        // BOTTOM FACE (Y=-50) - 4 vertices
        -50.0f, -50.0f,  50.0f,  0.8f, 0.7f, 0.4f,  0.0f, 0.0f,  // 0  (light brown)
        50.0f, -50.0f,  50.0f,  0.8f, 0.7f, 0.4f,  1.0f, 0.0f,  // 1  (light brown)
        50.0f, -50.0f, -50.0f,  0.8f, 0.7f, 0.4f,  1.0f, 1.0f,  // 2  (light brown)
        -50.0f, -50.0f, -50.0f,  0.8f, 0.7f, 0.4f,  0.0f, 1.0f,  // 3  (light brown)
        
        // TOP FACE (Y=50) - 4 vertices
        -50.0f,  50.0f,  50.0f,  0.9f, 0.9f, 0.9f,  0.0f, 0.0f,  // 4  (white)
        50.0f,  50.0f,  50.0f,  0.9f, 0.9f, 0.9f,  1.0f, 0.0f,  // 5  (white)
        50.0f,  50.0f, -50.0f,  0.9f, 0.9f, 0.9f,  1.0f, 1.0f,  // 6  (white)
        -50.0f,  50.0f, -50.0f,  0.9f, 0.9f, 0.9f,  0.0f, 1.0f,  // 7  (white)
        
        // FRONT FACE (+Z) - 4 vertices
        -50.0f, -50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 8  (red)
        50.0f, -50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 9  (red)
        50.0f,  50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 10 (red)
        -50.0f,  50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 11 (red)
        
        // BACK FACE (-Z) - 4 vertices
        50.0f, -50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 12 (green)
        -50.0f, -50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 13 (green)
        -50.0f,  50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 14 (green)
        50.0f,  50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 15 (green)
        
        // LEFT FACE (-X) - 4 vertices
        -50.0f, -50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 16 (blue)
        -50.0f, -50.0f,  50.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 17 (blue)
        -50.0f,  50.0f,  50.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 18 (blue)
        -50.0f,  50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 19 (blue)
        
        // RIGHT FACE (+X) - 4 vertices
        50.0f, -50.0f,  50.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 20 (yellow)
        50.0f, -50.0f, -50.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 21 (yellow)
        50.0f,  50.0f, -50.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 22 (yellow)
        50.0f,  50.0f,  50.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 23 (yellow)
    };
    // clang-format on

    // Index buffer - 6 faces, 2 triangles per face, 3 indices per triangle
    // clang-format off
    unsigned int indices[] = {
        // BOTTOM
        0, 1, 2,
        0, 2, 3,
        // TOP
        4, 5, 6,
        4, 6, 7,
        // FRONT
        8, 9, 10,
        8, 10, 11,
        // BACK
        12, 13, 14,
        12, 14, 15,
        // LEFT
        16, 17, 18,
        16, 18, 19,
        // RIGHT
        20, 21, 22,
        20, 22, 23
    };
    // clang-format on

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // ======================= VAO / VBO / EBO =======================
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    // Define vertex layout: position (3 floats) + color (3 floats)
    VertexBufferLayout layout;
    layout.Push<float>(3); // Position (x, y, z)
    layout.Push<float>(3); // Color (r, g, b)
    layout.Push<float>(2); // texture (x, y)

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int)); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= TRANSFORMATIONS SETUP =======================

    // one to one pixel mapping
    // glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, -30.0f, -305.0f));
    proj = glm::perspective(glm::radians(45.0f), ((float)width / (float)height), 0.1f, 1000.0f); // if something is closer to 0.1f it would be cliped and further than 100

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    Shader shader("../../src/1-getting-started/5-transformations/5.5-cube/shaders/Shaders.shaders");
    shader.Bind();

    // ======================= TEXTURE SETUP =======================

    Texture texture("../../res/textures/grass_block_side.png"); // GL_NEAREST for it to works
    texture.Bind();
    // this has to match SetUniform1i (default 0)

    shader.SetUniform1i("u_Texture", 0);

    //! Unbind everything (good practice to avoid accidental modifications)
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

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

    // ======================= VARIABES FOR LOOP =======================

    float r = 1.0f; // esto quedo por la escala, no lo saco porque tengo que sacar el uniform y bla bla...

    bool modelRotationBool = false;
    float modelRotation = 0.0f;
    double prevTime = glfwGetTime();
    bool rotationX = false;
    bool rotationY = false;
    bool rotationZ = false;

    glm::vec3 modelTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewTranslation = glm::vec3(0.0f, 0.0f, 0.0f);

    GLCall(glEnable(GL_DEPTH_TEST));
    //! you need to change the glClear in Renderer.cpp

    // ======================= RENDER LOOP =======================

    while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);

      updateTitle(window, "MyFirstWindow"); //* This is only for the fps

      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // Activate shader and set uniform
      shader.Bind(); // Get uniform location (must be done after shader program is active)
      shader.SetUniform1f("u_Scale", r);

      // ===== cube movement =====
      double crntTime = glfwGetTime();
      if (crntTime - prevTime >= 1 / 60 && modelRotationBool)
      {
        modelRotation += 0.2f;
        prevTime = crntTime;
      }

      //! this is not a good way of rendering a lot of things, what you what to do is batch rendering in a single draw call
      {
        model = glm::translate(glm::mat4(1.0f), modelTranslation);
        if (rotationX)
          model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(1.0f, 0.0f, 0.0f));
        if (rotationY)
          model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 1.0f, 0.0f));
        if (rotationZ)
          model = glm::rotate(model, glm::radians(modelRotation), glm::vec3(0.0f, 0.0f, 1.0f));

        view = glm::translate(view, viewTranslation);

        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
      }

      // ========================= ImGui =========================
      {
        ImGui::SliderFloat3("Translation X, Y, Z", &modelTranslation.x, -100.0f, 100.0f);
        ImGui::SliderFloat3("View Translation X, Y, Z", &viewTranslation.x, -2.0f, 2.0f);
        ImGui::SliderFloat("Rotation Y", &modelRotation, 0.0f, 360.0f);
        if (ImGui::Button("rotationX"))
        {
          rotationX = true;
          rotationY = false;
          rotationZ = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("rotationY"))
        {
          rotationX = false; // ← Cambiar a false
          rotationY = true;  // ← Cambiar a true
          rotationZ = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("rotationZ"))
        {
          rotationX = false;
          rotationY = false;
          rotationZ = true;
        }
        ImGui::SameLine();

        if (ImGui::Button("enable"))
          modelRotationBool = true;
        ImGui::SameLine();
        if (ImGui::Button("disable"))
          modelRotationBool = false;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  } // there is no need to delete anything because of the scope
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();

  return 0;
}
