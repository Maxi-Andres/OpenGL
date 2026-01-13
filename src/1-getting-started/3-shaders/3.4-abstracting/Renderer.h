#include <GLFW/glfw3.h>

//! no deberia estar aca, pero por solo esto crear otro archivo
// ======================= ERROR HANDLING =======================
#define ASSERT(x) \
  if (!(x))       \
    __debugbreak();
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);
