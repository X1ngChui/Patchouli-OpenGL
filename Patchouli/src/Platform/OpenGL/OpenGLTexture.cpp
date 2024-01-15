#include "Platform/OpenGL/OpenGLTexture.h"
#include "stb_image.h"

namespace Pache
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: width(width), height(height)
	{
		internalfmt = GL_RGBA8, datafmt = GL_RGBA;

		Log::coreAssert(internalfmt && datafmt, "Unsupported format.");

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, internalfmt, width, height);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path)
		: path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.generic_string().c_str(), &width, &height, &channels, 0);
		Log::coreAssert(data, "Texture loss at {}", path.string());

		this->width = static_cast<uint32_t>(width);
		this->height = static_cast<uint32_t>(height);

		internalfmt = 0, datafmt = 0;
		if (channels == 4)
		{
			internalfmt = GL_RGB8;
			datafmt = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalfmt = GL_RGB8;
			datafmt = GL_RGB;
		}

		Log::coreAssert(internalfmt && datafmt, "Unsupported format.");

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, internalfmt, width, height);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(texture, 0, 0, 0, width, height, datafmt, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &texture);
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		uint32_t bpp = datafmt == GL_RGBA ? 4 : 3;
		Log::coreAssert(size == width * height * bpp, "The data must be the entire texture.");
		glTextureSubImage2D(texture, 0, 0, 0, width, height, datafmt, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, texture);
	}
}