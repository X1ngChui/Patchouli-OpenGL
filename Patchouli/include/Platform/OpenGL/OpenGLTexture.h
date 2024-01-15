#pragma once
#include "Renderer/Texture.h"
#include "glad/glad.h"

namespace Pache
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::filesystem::path& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return width; }
		virtual uint32_t getHeight() const override { return height; }

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot) const override;
	private:
		std::filesystem::path path;
		uint32_t width;
		uint32_t height;
		uint32_t texture;
		GLenum internalfmt, datafmt;
	};
}