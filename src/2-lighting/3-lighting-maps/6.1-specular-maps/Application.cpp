#include "glad/glad.h" // with this the ouput doesn't say that it has linking problems
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

#include "Camera.h"

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

const unsigned int width = 1000;
const unsigned int height = 1000;

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

    // Interleaved vertex data: position (x, y, z) + color (r, g, b) + texture (x, y) + normals (x, y, z)
    // clang-format off
    float vertices[] = {
            // BOTTOM FACE (Y = -50) Normal points DOWN (0, -1, 0)
            -50.0f, -50.0f,  50.0f,   0.8f, 0.7f, 0.4f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f, // Color: light brown (0.8, 0.7, 0.4)
            50.0f, -50.0f,  50.0f,   0.8f, 0.7f, 0.4f,   1.0f, 0.0f,   0.0f, -1.0f, 0.0f, 
            50.0f, -50.0f, -50.0f,   0.8f, 0.7f, 0.4f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f, 
            -50.0f, -50.0f, -50.0f,   0.8f, 0.7f, 0.4f,   0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

            // TOP FACE (Y = +50) Normal points UP (0, 1, 0)
            -50.0f,  50.0f,  50.0f,   0.9f, 0.9f, 0.9f,   0.0f, 0.0f,   0.0f,  1.0f, 0.0f, // Color: white (0.9, 0.9, 0.9)
            50.0f,  50.0f,  50.0f,   0.9f, 0.9f, 0.9f,   1.0f, 0.0f,   0.0f,  1.0f, 0.0f, 
            50.0f,  50.0f, -50.0f,   0.9f, 0.9f, 0.9f,   1.0f, 1.0f,   0.0f,  1.0f, 0.0f, 
            -50.0f,  50.0f, -50.0f,   0.9f, 0.9f, 0.9f,   0.0f, 1.0f,   0.0f,  1.0f, 0.0f,

            // FRONT FACE (Z = +50) Normal points FORWARD (0, 0, 1)
            -50.0f, -50.0f,  50.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // Color: red (1.0, 0.0, 0.0)
            50.0f, -50.0f,  50.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 
            50.0f,  50.0f,  50.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f, 
            -50.0f,  50.0f,  50.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,

            // BACK FACE (Z = -50) Normal points BACKWARD (0, 0, -1)
            50.0f, -50.0f, -50.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f, // Color: green (0.0, 1.0, 0.0)
            -50.0f, -50.0f, -50.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
            -50.0f,  50.0f, -50.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
            50.0f,  50.0f, -50.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, -1.0f, 

            // LEFT FACE (X = -50) Normal points LEFT (-1, 0, 0)
            -50.0f, -50.0f, -50.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f, // Color: blue (0.0, 0.0, 1.0)
            -50.0f, -50.0f,  50.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
            -50.0f,  50.0f,  50.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
            -50.0f,  50.0f, -50.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

            // RIGHT FACE (X = +50) Normal points RIGHT (1, 0, 0)
            50.0f, -50.0f,  50.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f, // Color: yellow (1.0, 1.0, 0.0)
            50.0f, -50.0f, -50.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 
            50.0f,  50.0f, -50.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f, 
            50.0f,  50.0f,  50.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 
        };
    // clang-format on

    // Index buffer - 6 faces, 2 triangles per face, 3 indices per triangle
    // clang-format off
    unsigned int indices[] = {
        // BOTTOM (Y = -50) - Viewed from BELOW, needs inverted order
        0, 2, 1,
        0, 3, 2,
        // TOP (Y = +50) - Viewed from ABOVE
        4, 5, 6,
        4, 6, 7,
        // FRONT (Z = +50) - Viewed from the front
        8, 9, 10,
        8, 10, 11,
        // BACK (Z = -50) - Viewed from behind
        12, 13, 14,
        12, 14, 15,
        // LEFT (X = -50) - Viewed from the left
        16, 17, 18,
        16, 18, 19,
        // RIGHT (X = +50) - Viewed from the right
        20, 21, 22,
        20, 22, 23
    };
    // clang-format on

    // clang-format off
    float lightVertices[] = {
        // BOTTOM FACE - 4 vertices
        -20.0f, -20.0f,  20.0f,  
        20.0f, -20.0f,  20.0f, 
        20.0f, -20.0f, -20.0f, 
        -20.0f, -20.0f, -20.0f,  
        
        // TOP FACE - 4 vertices
        -20.0f,  20.0f,  20.0f, 
        20.0f,  20.0f,  20.0f,  
        20.0f,  20.0f, -20.0f, 
        -20.0f,  20.0f, -20.0f,  
    };
    // clang-format on

    // clang-format off
    unsigned int lightIndices[] = {
        // BOTTOM FACE (Y = -20)
        0, 2, 1,
        0, 3, 2,
        // FRONT FACE (Z = +20)
        0, 1, 5,
        0, 5, 4,
        // RIGHT FACE (X = +20)
        1, 2, 6,
        1, 6, 5,
        // BACK FACE (Z = -20)
        2, 3, 7,
        2, 7, 6,
        // LEFT FACE (X = -20)
        3, 0, 4,
        3, 4, 7,
        // TOP FACE (Y = +20)
        4, 5, 6,
        4, 6, 7
    };
    // clang-format on

    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST)); //! this is important for depth
    GLCall(glEnable(GL_CULL_FACE));

    // FACE CULLING: Doesn't render back faces (optimization + visual correction)
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK)); // Discards back faces (backfaces)
    GLCall(glFrontFace(GL_CCW)); // Front faces = vertices in counter-clockwise order
    //! the order of the indices matter if you use this

    // ======================= VAO / VBO / EBO =======================
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position (x, y, z)
    layout.Push<float>(3); // Color (r, g, b)
    layout.Push<float>(2); // texture (x, y)
    layout.Push<float>(3); // Normals (x, y, z)

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int)); //? maybe it could just be indices and in the method you do sizeof()

    // ======================= TRANSFORMATIONS SETUP =======================

    Camera camera(width, height, glm::vec3(0.0f, -30.0f, 305.0f));

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 modelTranslation = glm::vec3(0.0f, 0.0f, 0.0f);

    // ======================= LIGHT SOURCE TRANSFORMATIONS =======================
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 lightModel = glm::mat4(1.0f);
    glm::vec3 lightTranslation = glm::vec3(80.0f, 92.0f, 220.0f);

    float ambientLight = 0.4f;

    lightModel = glm::translate(lightModel, lightTranslation);

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    Shader shader("../../src/2-lighting/3-lighting-maps/6.1-specular-maps/shaders/Shaders.shaders");
    shader.Bind();

    // ======================= TEXTURE SETUP =======================

    Texture texture("../../res/textures/container2.png"); //! GL_NEAREST for it to work
    texture.Bind();
    // this has to match SetUniform1i (default 0)

    Texture specular("../../res/textures/container2_specular.png");
    specular.Bind(1);

    shader.SetUniform1f("u_Scale", 1.0f);
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniform1i("u_SpecularMap", 1);

    //! Unbind everything (good practice to avoid accidental modifications)
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    // ======================= LIGHT =======================
    Shader lightShader("../../src/2-lighting/3-lighting-maps/6.1-specular-maps/shaders/LightShaders.shaders");
    lightShader.Bind();

    VertexArray lightVAO;
    VertexBuffer lightVBO(lightVertices, sizeof(lightVertices));

    // Define vertex layout: position (3 floats)
    VertexBufferLayout lightLayout;
    lightLayout.Push<float>(3); // Position (x, y, z)

    lightVAO.AddBuffer(lightVBO, lightLayout);

    IndexBuffer lightIBO(lightIndices, sizeof(lightIndices) / sizeof(unsigned int));

    lightShader.Unbind();
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightIBO.Unbind();

    // ======================= ImGUI=======================

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // ======================= VARIABLES FOR LOOP =======================

    bool modelRotationBool = false;
    glm::vec3 modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);

    double prevTime = glfwGetTime();
    bool rotationX = false;
    bool rotationY = false;
    bool rotationZ = false;

    // ======================= RENDER LOOP =======================

    while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);

      updateTitle(window, "Crazy Sun"); //* This is only for the fps

      renderer.Clear();
      //! you need to change the glClear in Renderer.cpp

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // Activate shader and set uniform
      shader.Bind(); // Get uniform location (must be done after shader program is active)
      // shader.SetUniform1f("u_Scale", r);

      // ===== cube movement =====
      double crntTime = glfwGetTime();
      if (crntTime - prevTime >= 1.0 / 60.0) // 60 FPS approx
      {
        if (modelRotationBool)
        {
          if (rotationX)
            modelRotation.x += 1.0f;
          if (rotationY)
            modelRotation.y += 1.0f;
          if (rotationZ)
            modelRotation.z += 1.0f;
        }
        prevTime = crntTime;
      }

      // ========== camera movement ==========
      camera.Inputs(window);
      glm::mat4 proj_view = camera.Matrix(90.f, 0.1f, 10000.0f);

      //! this is not a good way of rendering a lot of things, what you want to do is batch rendering in a single draw call, calling multiple times drawing is bad!
      {
        texture.Bind(0); //! hay que bindear cuando son mas de 1 textura porque si no rompe
        specular.Bind(1);

        // this way I don't calculate radians in the loop
        float radX = glm::radians(modelRotation.x);
        float radY = glm::radians(modelRotation.y);
        float radZ = glm::radians(modelRotation.z);

        //? remember that these matrix calculations are read from right to left, meaning also from bottom to top, look where the variables are within the formula
        model = glm::translate(glm::mat4(1.0f), modelTranslation);
        // The recommended order is usually Y -> X -> Z to avoid common Gimbal Lock problems
        model = glm::rotate(model, radY, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, radX, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, radZ, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 mvp = proj_view * model; // column-major ordering because of OpenGL so the order is this one and not m * v * p
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("u_Model", model);
        shader.SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        shader.SetUniform3f("u_LightTranslation", lightTranslation.x, lightTranslation.y, lightTranslation.z);
        shader.SetUniform1f("u_AmbientLight", ambientLight);
        shader.SetUniform3f("u_CameraPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        renderer.Draw(va, ib, shader);
      }

      // ========== light source ==========
      {
        lightShader.Bind();

        lightModel = glm::translate(glm::mat4(1.0f), lightTranslation);

        glm::mat4 mvp = proj_view * lightModel;
        lightShader.SetUniformMat4f("u_MVP", mvp);
        lightShader.SetUniform4f("u_LightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        renderer.Draw(lightVAO, lightIBO, lightShader);
      }

      // ========================= ImGui =========================
      {
        ImGui::Begin("Renderer Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        // 1. Stats
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Performance");
        ImGui::Text("Avg: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::Separator();

        // 2. Model
        if (ImGui::CollapsingHeader("Model Settings"))
        {
          ImGui::Text("Main Cube");
          ImGui::DragFloat3("Position", &modelTranslation.x, 1.0f, -1000.0f, 1000.0f); // slider is very sensitive, dragfloat is more easy to use

          ImGui::Dummy(ImVec2(0.0f, 5.0f)); // small space
          ImGui::Text("Manual Rotation");
          ImGui::DragFloat3("Models Rotation", &modelRotation.x, 1.0f, -1000.0f, 1000.0f);
        }

        // 3. Auto Rotation Section
        if (ImGui::CollapsingHeader("Auto Rotation"))
        {
          ImGui::Text("Active Axes:");
          ImGui::Checkbox("X##rot", &rotationX);
          ImGui::SameLine();
          ImGui::Checkbox("Y##rot", &rotationY);
          ImGui::SameLine();
          ImGui::Checkbox("Z##rot", &rotationZ);

          float buttonWidth = ImGui::GetContentRegionAvail().x * 0.5f - 5.0f;
          if (ImGui::Button("Enable All", ImVec2(buttonWidth, 0)))
            modelRotationBool = true;
          ImGui::SameLine();
          if (ImGui::Button("Disable All", ImVec2(buttonWidth, 0)))
            modelRotationBool = false;
        }

        // 4. Light & Environment Section
        if (ImGui::CollapsingHeader("Light & Environment", ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::DragFloat3("Light Position", &lightTranslation.x, 1.0f, -1000.0f, 1000.0f);
          ImGui::ColorEdit4("Light Color", &lightColor.x);

          ImGui::SliderFloat("Ambient Intensity", &ambientLight, 0.0f, 1.0f);
        }

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