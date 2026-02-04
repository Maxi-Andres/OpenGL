#include <glad/glad.h> //! SIEMPRE ESTO ARRIBA DE TODO SI NO NO ANDA, si no pongo esto se rompe pero cherno no lo pone, debe ser por visual

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
  // opengl expects the texture to start at the bottom left, not the top left, with this we invert this
  stbi_set_flip_vertically_on_load(1);
  // lo cambie a 0 y ahora esta bien no se que onda
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4 because we want RGBA

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  // if you don't specify this parameters you're going to have a black texture, it doesn't have defaults
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_LocalBuffer)
    stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
  GLCall(glDeleteTextures(1, &m_RendererID)); // delete the texture from the GPU
}

void Texture::Bind(unsigned int slot) const
{
  //! REMEMBER STATE MACHINE
  // usually you wont be using the 32 texture it has except is a really big game (also the amount of textures that the system supports depents, it may not be all 32)
  // but usually there is ways to reuse texture, so you wont use all slots
  GLCall(glActiveTexture(GL_TEXTURE0 + slot)); // this is the slot 0 in where the next texture that i bind will be bind to this slot
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}
void Texture::Unbind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}