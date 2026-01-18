#include "glad/glad.h" // with this the ouput doesn't say that it has linking problems
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include <iomanip>

float mDeltaTime = 0.0f;
float mLastFrame = 0.0f;
float mPreviousTime = 0.0f;
int mFrameCount = 0;
float mFPS = 0.0f;

void updateTitle(GLFWwindow *window, const std::string &title);

float getFPS()
{
  float currentFrame = glfwGetTime();
  mDeltaTime = currentFrame - mLastFrame;
  mLastFrame = currentFrame;
  mFrameCount++;

  if (currentFrame - mPreviousTime >= 1.0f)
  {
    mFPS = mFrameCount / (currentFrame - mPreviousTime);
    mPreviousTime = currentFrame;
    mFrameCount = 0;
  }

  return mFPS;
}

void updateTitle(GLFWwindow *window, const std::string &title)
{
  float fps = getFPS();
  std::stringstream ss;
  ss << title << " | FPS: " << std::fixed << std::setprecision(2) << fps;
  std::string newTitle = ss.str();
  glfwSetWindowTitle(window, newTitle.c_str());
}