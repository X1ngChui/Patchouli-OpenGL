#pragma once
#include "Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"	

namespace Pache
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* window;
	};
}