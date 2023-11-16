#include "GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "glad/glad.h"

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
			Log::coreAssert(glfwInit(), "glfw initialization failed.");
			s_GLFWInitialized = true;
		}

		window = glfwCreateWindow((int)props.width, (int)props.height, attrib.title.c_str(), nullptr, nullptr);
		
		glfwMakeContextCurrent(window);
		Log::coreAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "glad initialization failed.");
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
				attrib.evtCallback(new WindowResizeEvent(width, height));
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				attrib.evtCallback(new WindowCloseEvent);
			});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					attrib.evtCallback(new KeyPressedEvent(key, false));
					break;
				}
				case GLFW_REPEAT:
				{
					attrib.evtCallback(new KeyPressedEvent(key, true));
					break;
				}
				case GLFW_RELEASE:
				{
					attrib.evtCallback(new KeyReleasedEvent(key));
					break;
				}
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				attrib.evtCallback(new KeyTypedEvent(character));
			});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					attrib.evtCallback(new MouseButtonPressedEvent(button));
					break;
				}
				case GLFW_RELEASE:
				{
					attrib.evtCallback(new MouseButtonReleasedEvent(button));
					break;
				}
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				attrib.evtCallback(new MouseScrolledEvent((float)xoffset, (float)yoffset));
			});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowAttrib& attrib = *(WindowAttrib*)glfwGetWindowUserPointer(window);
				attrib.evtCallback(new MouseMovedEvent((float)xpos, (float)ypos));
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