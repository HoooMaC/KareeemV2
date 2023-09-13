#include "Core/Kareeem.h"
#include "Platform.h"

#include <GLFW/glfw3.h>

namespace Karem {

    float Platform::GetTime()
    {
        return (float)glfwGetTime();
    }

}