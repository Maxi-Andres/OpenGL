#pragma once
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
  unsigned int m_RendererID;
  unsigned int m_Count;

public:
  IndexBuffer(const unsigned int *data, unsigned int count); // podes cambiar el tipo de dato de data para mejorar performance
  ~IndexBuffer();
  // Cuando el objeto de la clase se destruye, debe llamar a glDeleteBuffers para liberar la memoria en la tarjeta de video y evitar fugas de memoria (memory leaks).

  void Bind() const;
  void Unbind() const;

  inline unsigned int GetCount() const { return m_Count; }
};