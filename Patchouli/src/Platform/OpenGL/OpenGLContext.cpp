#include "Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Pache
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: window(window)
	{
	}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(window);
		Log::coreAssert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "glad initialization failed.");
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(window);
	}
}