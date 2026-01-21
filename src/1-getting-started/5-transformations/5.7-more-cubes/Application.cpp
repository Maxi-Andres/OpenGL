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

const unsigned int width = 1100;
const unsigned int height = 1100;

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

    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y)
    // clang-format off
    float vertices[] = {
        // BOTTOM FACE (Y=-50) - 4 vertices
        -50.0f, -50.0f,  50.0f,  0.8f, 0.7f, 0.4f,  0.0f, 0.0f,  // 0  (light brown)
        50.0f, -50.0f,  50.0f,  0.8f, 0.7f, 0.4f,  0.5f, 0.0f,  // 1  (light brown)
        50.0f, -50.0f, -50.0f,  0.8f, 0.7f, 0.4f,  0.5, 0.5f,  // 2  (light brown)
        -50.0f, -50.0f, -50.0f,  0.8f, 0.7f, 0.4f,  0.0f, 0.5f,  // 3  (light brown)
        
        // TOP FACE (Y=50) - 4 vertices
        -50.0f,  50.0f,  50.0f,  0.9f, 0.9f, 0.9f,  0.5f, 0.5f,  // 4  (white)
        50.0f,  50.0f,  50.0f,  0.9f, 0.9f, 0.9f,  1.0f, 0.5f,  // 5  (white)
        50.0f,  50.0f, -50.0f,  0.9f, 0.9f, 0.9f,  1.0f, 1.0f,  // 6  (white)
        -50.0f,  50.0f, -50.0f,  0.9f, 0.9f, 0.9f,  0.5f, 1.0f,  // 7  (white)
        
        // FRONT FACE (+Z) - 4 vertices
        -50.0f, -50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.5f, 0.5f,  // 8  (red)
        50.0f, -50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.5f,  // 9  (red)
        50.0f,  50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 10 (red)
        -50.0f,  50.0f,  50.0f,  1.0f, 0.0f, 0.0f,  0.5f, 1.0f,  // 11 (red)
        
        // BACK FACE (-Z) - 4 vertices
        50.0f, -50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.5f, 0.5f,  // 12 (green)
        -50.0f, -50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.5f,  // 13 (green)
        -50.0f,  50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 14 (green)
        50.0f,  50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.5f, 1.0f,  // 15 (green)
        
        // LEFT FACE (-X) - 4 vertices
        -50.0f, -50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f,  // 16 (blue)
        -50.0f, -50.0f,  50.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.5f,  // 17 (blue)
        -50.0f,  50.0f,  50.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 18 (blue)
        -50.0f,  50.0f, -50.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,  // 19 (blue)
        
        // RIGHT FACE (+X) - 4 vertices
        50.0f, -50.0f,  50.0f,  1.0f, 1.0f, 0.0f,  0.5f, 0.5f,  // 20 (yellow)
        50.0f, -50.0f, -50.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.5f,  // 21 (yellow)
        50.0f,  50.0f, -50.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 22 (yellow)
        50.0f,  50.0f,  50.0f,  1.0f, 1.0f, 0.0f,  0.5f, 1.0f,  // 23 (yellow)
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
    glm::mat4 baseView = glm::translate(view, glm::vec3(0.0f, -30.0f, -305.0f));
    proj = glm::perspective(glm::radians(90.0f), ((float)width / (float)height), 0.1f, 100000.0f); // if something is closer to 0.1f it would be cliped and further than 100

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    Shader shader("../../src/1-getting-started/5-transformations/5.7-more-cubes/shaders/Shaders.shaders");
    shader.Bind();

    // ======================= TEXTURE SETUP =======================

    Texture texture("../../res/textures/grass_block_atlas.png"); // GL_NEAREST for it to works
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
    float modelRotationX = 0.0f;
    float modelRotationY = 0.0f;
    float modelRotationZ = 0.0f;

    double prevTime = glfwGetTime();
    bool rotationX = false;
    bool rotationY = false;
    bool rotationZ = false;

    glm::vec3 modelTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewTranslation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3(200.0f, 500.0f, -1500.0f),
        glm::vec3(-150.0f, -220.0f, -250.0f),
        glm::vec3(-380.0f, -200.0f, -1230.0f),
        glm::vec3(240.0f, -40.0f, -350.0f),
        glm::vec3(-170.0f, 300.0f, -750.0f),
        glm::vec3(130.0f, -200.0f, -250.0f),
        glm::vec3(150.0f, 200.0f, -250.0f),
        glm::vec3(150.0f, 20.0f, -150.0f),
        glm::vec3(-130.0f, 100.0f, -150.0f)};

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
      if (crntTime - prevTime >= 1.0 / 60.0) // 60 FPS aprox
      {
        if (modelRotationBool)
        {
          if (rotationX)
            modelRotationX += 1.0f;
          if (rotationY)
            modelRotationY += 1.0f;
          if (rotationZ)
            modelRotationZ += 1.0f;
        }
        prevTime = crntTime;
      }

      //! this is not a good way of rendering a lot of things, what you what to do is batch rendering in a single draw call, calling multiple times drawing is bad!
      {
        model = glm::translate(glm::mat4(1.0f), modelTranslation);
        // El orden recomendado suele ser Y -> X -> Z para evitar problemas de Gimbal Lock comunes
        model = glm::rotate(model, glm::radians(modelRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(modelRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(modelRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

        // view = glm::translate(baseView, viewTranslation); // si le pones v

        glm::mat4 mvp = proj * view * model; // column-major ordering because of OpenGl so the orther is this one and not m * v * p
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);

        for (unsigned int i = 0; i < 9; i++)
        {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          float angle = 20.0f * i;
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          model = glm::rotate(model, glm::radians(modelRotationX), glm::vec3(1.0f, 0.0f, 0.0f));
          model = glm::rotate(model, glm::radians(modelRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
          model = glm::rotate(model, glm::radians(modelRotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

          view = glm::translate(baseView, viewTranslation); // si le pones v

          glm::mat4 mvp = proj * view * model; // column-major ordering because of OpenGl so the orther is this one and not m * v * p
          shader.SetUniformMat4f("u_MVP", mvp);

          renderer.Draw(va, ib, shader);
        }
      }

      // ========================= ImGui =========================
      {
        ImGui::Begin("Controls");
        ImGui::SliderFloat3("Model Pos", &modelTranslation.x, -200.0f, 200.0f);
        ImGui::SliderFloat3("View Offset", &viewTranslation.x, -1000.0f, 1000.0f);

        ImGui::Separator();
        ImGui::Text("Manual Rotation");
        ImGui::SliderFloat("Rot X", &modelRotationX, -360.0f, 360.0f);
        ImGui::SliderFloat("Rot Y", &modelRotationY, -360.0f, 360.0f);
        ImGui::SliderFloat("Rot Z", &modelRotationZ, -360.0f, 360.0f);

        ImGui::Separator();
        ImGui::Text("Auto Rotation Settings");
        // Usamos Checkbox en lugar de botones con continue
        ImGui::Checkbox("Rotate X", &rotationX);
        ImGui::SameLine();
        ImGui::Checkbox("Rotate Y", &rotationY);
        ImGui::SameLine();
        ImGui::Checkbox("Rotate Z", &rotationZ);

        if (ImGui::Button("Enable All"))
          modelRotationBool = true;
        ImGui::SameLine();
        if (ImGui::Button("Disable All"))
          modelRotationBool = false;

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
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
