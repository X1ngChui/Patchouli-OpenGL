#pragma once
#include "Renderer/Texture.h"

namespace Pache
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::filesystem::path& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return width; }
		virtual uint32_t getHeight() const override { return height; }

		virtual void bind(uint32_t slot) const override;
	private:
		std::filesystem::path path;
		uint32_t width;
		uint32_t height;
		uint32_t texture;
	};
}