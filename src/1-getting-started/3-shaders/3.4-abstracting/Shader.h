#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader
{
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  Shader(const std::string &filepath); // remember that the vertex and fragment shaders are in the same file
  ~Shader();

  void Bind() const; // the name is bind just for consistence in the api that we are making
  void Unbind() const;

  // Set uniforms
  void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
  void SetUniform1f(const std::string &name, float v0);

private:
  ShaderProgramSource ParseShader(const std::string &filepath);
  unsigned int CompileShader(unsigned int type, const std::string &source);
  unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
  unsigned int GetUniformLocation(const std::string &name);
};
