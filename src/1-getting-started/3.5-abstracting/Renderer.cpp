#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include <iostream>

void GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GLLogCall(const char *function, const char *file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " | " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::Clear() const
{
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader) const
{
  // Activate shader and set uniform
  shader.Bind(); // Get uniform location (must be done after shader program is active)

  // Bind VAO and index buffer, then draw
  va.Bind();
  ib.Bind();

  GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0));

  // In a game engine, you don't unbind because basically whenever you want to draw something else, you're going to bind it, so all you're doing is losing performance.
}