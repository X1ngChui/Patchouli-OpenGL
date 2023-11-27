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

		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
	private:
		static GraphicsAPI api;
	};
}