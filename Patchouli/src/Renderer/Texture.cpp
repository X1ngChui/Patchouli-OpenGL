#include "Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Renderer.h"

namespace Pache
{
	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLTexture2D>(width, height);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}

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

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: texture(texture)
	{
		textureCoords[0] = { min.x, min.y };
		textureCoords[1] = { max.x, min.y };
		textureCoords[2] = { max.x, max.y };
		textureCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min = { coords.x * cellSize.x / texture->getWidth(), coords.y * cellSize.y / texture->getHeight() };
		glm::vec2 max = { (coords.x + spriteSize.x) * cellSize.x / texture->getWidth(), (coords.y + spriteSize.y) * cellSize.y / texture->getHeight() };
		
		return makeIntrusive<SubTexture2D>(texture, min, max);
	}
}