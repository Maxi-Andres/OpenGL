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
    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y)
    // clang-format off
    float vertices[] = {
        // CARA BASE (2 triángulos que forman un cuadrado)
        // Triángulo 1 de la base
        -50.0f,  0.0f,  50.0f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  // 0
        -50.0f,  0.0f, -50.0f,    0.83f, 0.70f, 0.44f,    1.0f, 0.0f,  // 1
        50.0f,  0.0f, -50.0f,    0.83f, 0.70f, 0.44f,    1.0f, 1.0f,  // 2
        
        // Triángulo 2 de la base
        -50.0f,  0.0f,  50.0f,    0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  // 3
        50.0f,  0.0f, -50.0f,    0.83f, 0.70f, 0.44f,    1.0f, 1.0f,  // 4
        50.0f,  0.0f,  50.0f,    0.83f, 0.70f, 0.44f,    0.0f, 1.0f,  // 5
        
        // CARA FRONTAL (triángulo)
        -50.0f,  0.0f,  50.0f,    0.92f, 0.86f, 0.76f,    0.0f, 0.0f,  // 6
        50.0f,  0.0f,  50.0f,    0.92f, 0.86f, 0.76f,    1.0f, 0.0f,  // 7
          0.0f, 80.0f,   0.0f,    0.92f, 0.86f, 0.76f,    0.5f, 1.0f,  // 8
        
        // CARA DERECHA (triángulo)
        50.0f,  0.0f,  50.0f,    0.92f, 0.86f, 0.76f,    0.0f, 0.0f,  // 9
        50.0f,  0.0f, -50.0f,    0.92f, 0.86f, 0.76f,    1.0f, 0.0f,  // 10
          0.0f, 80.0f,   0.0f,    0.92f, 0.86f, 0.76f,    0.5f, 1.0f,  // 11
        
        // CARA TRASERA (triángulo)
        50.0f,  0.0f, -50.0f,    0.92f, 0.86f, 0.76f,    0.0f, 0.0f,  // 12
        -50.0f,  0.0f, -50.0f,    0.92f, 0.86f, 0.76f,    1.0f, 0.0f,  // 13
          0.0f, 80.0f,   0.0f,    0.92f, 0.86f, 0.76f,    0.5f, 1.0f,  // 14
        
        // CARA IZQUIERDA (triángulo)
        -50.0f,  0.0f, -50.0f,    0.92f, 0.86f, 0.76f,    0.0f, 0.0f,  // 15
        -50.0f,  0.0f,  50.0f,    0.92f, 0.86f, 0.76f,    1.0f, 0.0f,  // 16
          0.0f, 80.0f,   0.0f,    0.92f, 0.86f, 0.76f,    0.5f, 1.0f,  // 17
    };
    // clang-format on

    // Index buffer for drawing two triangles (a quad) this has to be an unsigned int
    unsigned int indices[] = {
        // Base
        0, 1, 2,
        3, 4, 5,

        // Caras laterales
        6, 7, 8,    // Frontal
        9, 10, 11,  // Derecha
        12, 13, 14, // Trasera
        15, 16, 17  // Izquierda
    };

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

    IndexBuffer ib(indices, 24); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= TRANSFORMATIONS SETUP =======================

    // one to one pixel mapping
    // glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    float rotation = 0.0f;

    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, -40.0f, -225.0f));
    proj = glm::perspective(glm::radians(45.0f), ((float)width / (float)height), 0.1f, 10000.0f); // if something is closer to 0.1f it would be cliped and further than 100

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    Shader shader("../../src/1-getting-started/5-transformations/5.4-pyramid/shaders/Shaders.shaders");
    shader.Bind(); // this has to match SetUniform1i (default 0)

    // ======================= TEXTURE SETUP =======================

    Texture texture("../../res/textures/catbop.png");
    texture.Bind();

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

    glm::vec3 translation(0, 0, 0);
    rotation = 0.0f;

    glm::vec3 viewtranslation(0, 0, 0);
    float viewangle = 0.0f;

    float r = 1.0f; // esto quedo por la escala, no lo saco porque tengo que sacar el uniform y bla bla...

    float levitation = 0.0f;
    float speed = 0.02f;
    bool ascending = true;

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

      rotation += 0.2f;

      if (ascending)
      {
        levitation += speed;
        if (levitation >= 20.0f)
          ascending = false;
      }
      else
      {
        levitation -= speed;
        if (levitation <= 0.0f)
          ascending = true;
      }

      translation.y = levitation;

      //! this is not a good way of rendering a lot of things, what you what to do is batch rendering in a single draw call
      {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // el que tiene 1.0f es el eje que rota x,y,z

        view = glm::translate(view, viewtranslation);
        view = glm::rotate(view, glm::radians(viewangle), glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(va, ib, shader);
      }

      {
        ImGui::SliderFloat3("Translation X, Y, Z", &translation.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat3("View Translation X, Y, Z", &viewtranslation.x, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation Y", &rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("Camara rotation", &viewangle, 0.0f, 360.0f);
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
