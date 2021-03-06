#include "Platform/Windows/WindowGLFW.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace Engine
{
    namespace
    {
        void framebufferResizeCallback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    }

    class WindowGLFW::Impl
    {
    public:
        Impl(const WindowProps &props)
        {
            if (!glfwInit())
            {
                std::cerr << "Error initializating GLFW" << std::endl;
                exit(EXIT_FAILURE);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            m_window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(),
                                        NULL, NULL);
            if (!m_window)
            {
                std::cerr << "Unable to create GLFW window" << std::endl;
                exit(EXIT_FAILURE);
            }

            glfwMakeContextCurrent(m_window);

            glfwSetFramebufferSizeCallback(m_window, (GLFWframebuffersizefun)framebufferResizeCallback);
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Failed to initialize GLAD" << std::endl;
                throw std::runtime_error("Failed to initialize GLAD");
            }
            glViewport(0, 0, props.Width, props.Height);
        }
        virtual ~Impl()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
        }

    public:
        void OnUpdate()
        {
            if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(m_window, true);
        }

        uint32_t GetWidth() const
        {
            int height, width;
            glfwGetFramebufferSize(m_window, &width, &height);
            return (uint32_t)width;
        }

        uint32_t GetHeight() const
        {
            int height, width;
            glfwGetFramebufferSize(m_window, &width, &height);
            return (uint32_t)height;
        }

        void *GetNativeWindow() const
        {
            return (void *)m_window;
        }

    private:
        GLFWwindow *m_window;
    }; // class WindowGLFW::impl

    //
    // --- WindowGLFW ---
    //
    WindowGLFW::WindowGLFW(const WindowProps &props)
        : m_impl(std::make_unique<WindowGLFW::Impl>(props))
    {
    }

    WindowGLFW::~WindowGLFW()
    {
    }

    void WindowGLFW::OnUpdate()
    {
        m_impl->OnUpdate();
    }

    uint32_t WindowGLFW::GetHeight() const
    {
        return m_impl->GetHeight();
    }

    uint32_t WindowGLFW::GetWidth() const
    {
        return m_impl->GetWidth();
    }

    void *WindowGLFW::GetNativeWindow() const
    {
        return m_impl->GetNativeWindow();
    }
} // namespace Engine
