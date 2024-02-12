#include "Platform/Windows/WindowsInput.h"
#include "GLFW/glfw3.h"
#include "Core/Application.h"

namespace Pache
{
	Input* Input::instance = new WindowsInput;

	bool WindowsInput::isKeyPressedImpl(int keycode) const
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().getNativeWindow();
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) const
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().getNativeWindow();
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::getMouseXImpl() const
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().getNativeWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)x;
	}

	float WindowsInput::getMouseYImpl() const
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().getNativeWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)y;
	}

	std::pair<float, float> WindowsInput::getMousePosImpl() const
	{
		GLFWwindow* window = (GLFWwindow*)Application::getApplication().getWindow().getNativeWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}
}