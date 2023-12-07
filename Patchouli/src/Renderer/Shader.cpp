#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pache
{
	Shader* Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLShader(vertexSource, fragmentSource);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}
}