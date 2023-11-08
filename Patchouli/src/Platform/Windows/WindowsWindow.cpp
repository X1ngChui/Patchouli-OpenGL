#include "GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

namespace Pache
{
	static bool s_GLFWInitialized = false;

	Window* Window::create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		attrib.title = props.title;
		attrib.width = props.width;
		attrib.height = props.height;

		if (!s_GLFWInitialized)
		{
			Log::coreAssert(glfwInit(), "GLFW initialization failed.");
			s_GLFWInitialized = true;
		}

		window = glfwCreateWindow((int)props.width, (int)props.height, attrib.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(window);

		glfwSetWindowUserPointer(window, &attrib);
		setVSync(true);

		// Set glfw callbacks
		glfwSetErrorCallback([](int error_code, const char* description)
			{
				Log::coreError("glfwError({0}): {1}", error_code, description);
			});

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				attrib.width = width;
				attrib.height = height;

				WindowResizeEvent evt(width, height);
				attrib.evtCallback(evt);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				WindowCloseEvent evt;
				attrib.evtCallback(evt);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent evt(key, false);
					attrib.evtCallback(evt);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent evt(key, true);
					attrib.evtCallback(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent evt(key);
					attrib.evtCallback(evt);
					break;
				}
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				KeyTypedEvent evt(character);
				attrib.evtCallback(evt);
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent evt(button);
					attrib.evtCallback(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent evt(button);
					attrib.evtCallback(evt);
					break;
				}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent evt((float)xoffset, (float)yoffset);
				attrib.evtCallback(evt);
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				MouseMovedEvent evt((float)xpos, (float)ypos);
				attrib.evtCallback(evt);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(window);
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (attrib.VSync = enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}
}