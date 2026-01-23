#pragma once
// #include <GLFW/glfw3.h>

class VertexBuffer
{
private:
  unsigned int m_RendererID;

public:
  VertexBuffer(const void *data, unsigned int size); // size is in bytes
  ~VertexBuffer();
  // Cuando el objeto de la clase se destruye, debe llamar a glDeleteBuffers para liberar la memoria en la tarjeta de video y evitar fugas de memoria (memory leaks).

  void Bind() const;
  void Unbind() const;
};