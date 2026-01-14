#include <glad/glad.h> //! SIEMPRE ESTO ARRIBA DE TODO SI NO NO ANDA, si no pongo esto se rompe pero cherno no lo pone, debe ser por visual
#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string &filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

// Parses a shader file containing both vertex and fragment shaders
// Expects #shader vertex and #shader fragment directives to separate the two
ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
  std::ifstream stream(filepath);

  if (!stream.is_open())
  {
    std::cout << "[ERROR] Failed to open shader file at: "
              << filepath << std::endl;
    return {};
  }

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    }
    else
    {
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

// Compiles an individual shader (vertex or fragment) and checks for GLSL syntax errors
unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
  GLCall(unsigned int id = glCreateShader(type));
  const char *src = source.c_str();
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  // Check for compilation errors
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char *message = (char *)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

// Compiles both vertex and fragment shaders, links them into a GPU program, validates it, and cleans up individual shaders
unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  // Delete individual shaders as they're now linked into the program
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
  GLCall(glUseProgram(0));
}
void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
//! ============ this one is temporal
void Shader::SetUniform1f(const std::string &name, float v0)
{
  GLCall(glUniform1f(GetUniformLocation(name), v0));
}
//! ============ this one is temporal

unsigned int Shader::GetUniformLocation(const std::string &name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

  m_UniformLocationCache[name] = location;
  return location;
}
