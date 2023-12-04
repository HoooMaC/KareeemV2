#include "Core/Kareeem.h"
#include "Platform/Platform.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Karem {

    float Platform::GetTime()
    {
        return (float)glfwGetTime();
    }

    void Platform::ResizeWindow(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        glfwSetWindowSize(window, width, height);
        glfwSetWindowMonitor(window, NULL, 0, 0, width, height, 0); // Kembali ke jendela biasa dengan resolusi 800x600

    }

}