#include <glad/glad.h>
#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
  GLCall(glGenVertexArrays(1, &m_RendererID));
}
VertexArray::~VertexArray()
{
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto &element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)(uintptr_t)offset));
    offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
  }
}

// glVertexAttribPointer(
//     index,      // Attribute location (0 = position, 1 = color, etc.)
//     count,      // Number of components (3 for vec3, 4 for vec4, etc.)
//     type,       // Data type (GL_FLOAT, GL_INT, etc.)
//     normalized, // Should normalize? (GL_TRUE/GL_FALSE)
//     stride,     // Bytes between consecutive vertices
//     offset      // Offset to first component
// );

void VertexArray::Bind() const
{
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
  GLCall(glBindVertexArray(0));
}