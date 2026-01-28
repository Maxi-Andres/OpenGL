#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "vendor/glm/gtx/rotate_vector.hpp"
#include "vendor/glm/gtx/vector_angle.hpp"

// #include "Shader.h"

class Camera
{
private:
  glm::vec3 m_Position;
  glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
  // glm::mat4 m_cameraMatrix = glm::mat4(1.0f);

  int m_Width;
  int m_Height;

  const float m_NormalVelocity = 0.6f;
  const float m_SprintVelocity = 0.2f;

  float m_Speed = m_NormalVelocity;
  float m_Sensitivity = 100.0f;

  bool m_FirstClick = true;

public:
  Camera(int width, int height, glm::vec3 position);
  ~Camera();

  glm::mat4 Matrix(float FOVdeg, float nearPlane, float farPlane);
  // glm::mat4 Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
  void Inputs(GLFWwindow *window);

  glm::vec3 GetPosition();
  void SetPosition(glm::vec3 newPosition);
};
