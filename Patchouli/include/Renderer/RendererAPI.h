#pragma once
#include "glm/glm.hpp"

namespace Pache
{
	class RendererAPI
	{
	public:
		enum GraphicsAPI
		{
			None = 0, OpenGL
		};

		static GraphicsAPI getGraphicsAPI() { return api; }

		virtual void init() = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) = 0;
	private:
		static GraphicsAPI api;
	};
}