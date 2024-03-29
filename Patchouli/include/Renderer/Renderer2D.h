#pragma once
#include "Texture.h"
#include "Renderer/Renderer.h"

namespace Pache
{
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();
		static void flush();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture,
			float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
			const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
			const glm::vec4& color);
		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t getVertexCount() const { return quadCount * 4; }
			uint32_t getVertexIndex() const { return quadCount * 6; }
		};
		static Statistics getStats();
		static void resetStats();

	private:
		static void flushAndReset();
	};
}