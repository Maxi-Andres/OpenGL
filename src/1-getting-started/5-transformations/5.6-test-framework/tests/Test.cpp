#include "Test.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

namespace test
{

  TestMenu::TestMenu(Test *&currentTestPointer)
      : m_CurrentTest(currentTestPointer)
  {
  }

  TestMenu::~TestMenu() {}

  void TestMenu::OnImGuiRender()
  {
    for (auto &test : m_Tests)
    {
      if (ImGui::Button(test.first.c_str()))
        m_CurrentTest = test.second();
    }
  }

}