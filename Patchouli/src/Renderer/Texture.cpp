#include "Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Pache
{
	Ref<Texture2D> Texture2D::create(const std::filesystem::path& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLTexture2D>(path);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}
}