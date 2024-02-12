#pragma once
#include "glm/glm.hpp"

namespace Pache
{
	class Texture : public RefCounted
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(uint32_t width, uint32_t height);
		static Ref<Texture2D> create(const std::filesystem::path& path);
	};

	class SubTexture2D : public RefCounted
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> getTexture() const { return texture; }
		const glm::vec2* getTextureCoords() const { return textureCoords; }

		static Ref<SubTexture2D> create(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	private:
		Ref<Texture2D> texture;
		glm::vec2 textureCoords[4];
	};
}