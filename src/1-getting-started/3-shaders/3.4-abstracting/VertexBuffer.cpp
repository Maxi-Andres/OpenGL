#include <glad/glad.h> //! SIEMPRE ESTO ARRIBA DE TODO SI NO NO ANDA, si no pongo esto se rompe pero cherno no lo pone, debe ser por visual
#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() // this destructor is called when the object escapes the scope in the code
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void VertexBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}