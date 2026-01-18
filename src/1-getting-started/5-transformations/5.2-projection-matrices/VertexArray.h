#pragma once
// #include "VertexBufferLayout.h" // ! el orden importa //lo comento por el include ciclico que hay
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
  unsigned int m_RendererID;

public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

  void Bind() const;
  void Unbind() const;
};
