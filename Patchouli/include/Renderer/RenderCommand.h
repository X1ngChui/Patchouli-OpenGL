#pragma once
#include "RendererAPI.h"

namespace Pache
{
	class RenderCommand
	{
	public:
		static void init()
		{
			api->init();
		}

		static void setClearColor(const glm::vec4& color)
		{
			api->setClearColor(color);
		}

		static void clear()
		{
			api->clear();
		}

		static void drawIndexed(const Ref<VertexArray>& vertexArray)
		{
			api->drawIndexed(vertexArray);
		}
	private:
		static RendererAPI* api;
	};
}