#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TestTexture2D.h"

#include "../Renderer.h" //! que render estara agarrando no?
#include "vendor/imgui/imgui.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace test
{
  TestTexture2D::TestTexture2D()
      : m_Proj(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(250, 250, 0), m_TranslationB(550, 550, 0)
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

    m_Shader = std::make_unique<Shader>("../../src/1-getting-started/5-transformations/5.6-test-framework/shaders/Shaders.shaders");
    m_VAO = std::make_unique<VertexArray>();

    // ======================= VAO / VBO / EBO =======================
    m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    // Define vertex layout: position (3 floats) + color (3 floats)
    VertexBufferLayout layout;
    layout.Push<float>(3); // Position (x, y, z)
    layout.Push<float>(3); // Color (r, g, b)
    layout.Push<float>(2); // texture (x, y)

    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = std::make_unique<IndexBuffer>(indices, 6);

    //? maybe it could just be indices and in the method you do sizeof()

    // ======================= TRANSFORMATIONS SETUP =======================

    // one to one pixel mapping
    // m_Proj = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1.0f, 1.0f);
    // m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    // ======================= SHADER SETUP =======================

    // this is from the build that's why ../
    m_Shader->Bind(); // this has to match SetUniform1i (default 0)

    // ======================= TEXTURE SETUP =======================

    m_Texture = std::make_unique<Texture>("../../res/textures/catbop.png");
    m_Texture->Bind();
    m_Shader->SetUniform1f("u_Scale", 1.0f);

    m_Shader->SetUniform1i("u_Texture", 0);

    //! Unbind everything (good practice to avoid accidental modifications)
    // no hace falta por el scope y alta paja
  }

  TestTexture2D::~TestTexture2D()
  {
  }

  void TestTexture2D::OnUpdate(float deltaTime)
  {
  }

  void TestTexture2D::OnRender()
  {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind();

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);

      renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);

      renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }
  }

  void TestTexture2D::OnImGuiRender()
  {
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 800.0f);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 800.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  }
}