#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Pache
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray;
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}
}