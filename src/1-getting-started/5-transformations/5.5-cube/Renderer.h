#pragma once
// #include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//! no deberia estar aca, pero por solo esto crear otro archivo
// ======================= ERROR HANDLING =======================
#define ASSERT(x) \
  if (!(x))       \
    __debugbreak();
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
private:
  /* data */
public:
  // Renderer(/* args */);
  // ~Renderer();
  void Clear() const;
  void Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader) const;
};
