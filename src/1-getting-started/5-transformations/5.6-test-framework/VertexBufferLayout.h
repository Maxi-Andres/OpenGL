#pragma once

#include <glad/glad.h>
#include <vector>
#include <GLFW/glfw3.h>

#include "Renderer.h"

struct VertexBufferElement
{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type)
  {
    switch (type)
    {
    case GL_FLOAT:
      return 4;
    case GL_UNSIGNED_INT:
      return 4;
    case GL_UNSIGNED_BYTE:
      return 1;
    }
    ASSERT(false);
    return 0;
  } // If you're getting here, it's because you passed an unsupported GL type. Abort the program so you can fix it.
};

class VertexBufferLayout
{
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;
  // m_Elements = [
  //     {type: GL_FLOAT, count: 3, normalized: false},  // Position
  //     {type: GL_FLOAT, count: 3, normalized: false}   // Color
  // ]
  // m_Stride = 24 bytes  // 6 floats × 4 bytes

public:
  VertexBufferLayout()
      : m_Stride(0) {}

  // ~VertexBufferLayout();

  template <typename T>
  void Push(unsigned int count)
  {
    static_assert(false);
  }

  template <>
  void Push<float>(unsigned int count)
  {
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
  }

  template <>
  void Push<unsigned int>(unsigned int count)
  {
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
  }

  template <> // esto es por bytes
  void Push<unsigned char>(unsigned int count)
  {
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
  }

  inline const std::vector<VertexBufferElement> GetElements() const & { return m_Elements; }
  inline unsigned int GetStride() const { return m_Stride; }
};