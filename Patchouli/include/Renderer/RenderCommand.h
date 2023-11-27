#pragma once
#include "RendererAPI.h"

namespace Pache
{
	class RenderCommand
	{
	public:
		static void setClearColor(const glm::vec4& color)
		{
			api->setClearColor(color);
		}

		static void clear()
		{
			api->clear();
		}

		static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			api->drawIndexed(vertexArray);
		}
	private:
		static RendererAPI* api;
	};
}