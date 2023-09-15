#include "Core/Kareeem.h"
#include "Platform.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Karem {

    float Platform::GetTime()
    {
        return (float)glfwGetTime();
    }

    void Platform::ResizeWindow(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

}