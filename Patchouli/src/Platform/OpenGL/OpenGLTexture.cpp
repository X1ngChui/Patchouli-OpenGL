#include "Platform/OpenGL/OpenGLTexture.h"
#include "stb_image.h"
#include "glad/glad.h"

namespace Pache
{
	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path path)
		: path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
		Log::coreAssert(data, "Texture loss at {}", path.string());

		this->width = static_cast<uint32_t>(width);
		this->height = static_cast<uint32_t>(height);

		GLenum ifmt = 0, dfmt = 0;
		if (channels == 4)
		{
			ifmt = GL_RGB8;
			dfmt = GL_RGBA;
		}
		else if (channels == 3)
		{
			ifmt = GL_RGB8;
			dfmt = GL_RGB;
		}
		Log::coreAssert(ifmt && dfmt, "Unsupported format.");

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, ifmt, width, height);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, dfmt, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &texture);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, texture);
	}
}