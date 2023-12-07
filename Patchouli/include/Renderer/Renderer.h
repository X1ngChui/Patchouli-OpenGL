#pragma once
#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "OrthographicCamera.h"

namespace Pache
{
	class Renderer
	{
	public:
		static void beginScene(OrthographicCamera& camera);
		static void endScence();
		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::GraphicsAPI getAPI() { return RendererAPI::getGraphicsAPI(); }
	private:
		struct Scene
		{
			glm::mat4 viewProjection;
		};

		static Scene* scene;
	};
}