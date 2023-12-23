#include "Renderer/Renderer.h"
#include "Renderer/IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Pache
{
	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLIndexBuffer>(indices, size);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}
}