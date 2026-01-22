#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
    : m_Width(width), m_Height(height), m_Position(position)
{
  // Camera::m_Width = width; // construyen las variables y luego se asignan, esto es peor
}

Camera::~Camera() {}

glm::mat4 Camera::Matrix(float FOVdeg, float nearPlane, float farPlane) //! pensa como hacerlo void
{
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
  projection = glm::perspective(glm::radians(FOVdeg), (float)m_Width / (float)m_Height, nearPlane, farPlane);

  return (projection * view);
}

void Camera::Inputs(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_Position += m_Speed * m_Orientation;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_Position += m_Speed * -m_Orientation;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_Position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_Position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up));

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    m_Position += m_Speed * m_Up;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    m_Position += m_Speed * -m_Up;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_Speed = m_NormalVelocity;
  else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    m_Speed = m_SprintVelocity;

  // Handles mouse inputs
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    // Hides mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Prevents camera from jumping on the first click
    if (m_FirstClick)
    {
      glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
      m_FirstClick = false;
    }

    // Stores the coordinates of the cursor
    double mouseX;
    double mouseY;
    // Fetches the coordinates of the cursor
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
    // and then "transforms" them into degrees
    float rotX = m_Sensitivity * (float)(mouseY - (m_Width / 2)) / m_Height;
    float rotY = m_Sensitivity * (float)(mouseX - (m_Height / 2)) / m_Width;

    // Calculates upcoming vertical change in the Orientation
    glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

    // Decides whether or not the next vertical Orientation is legal or not
    if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
    {
      m_Orientation = newOrientation;
    }

    // Rotates the Orientation left and right
    m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

    // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
    glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
  }
  else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
  {
    // Unhides cursor since camera is not looking around anymore
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // Makes sure the next time the camera looks around it doesn't jump
    m_FirstClick = true;
  }
}