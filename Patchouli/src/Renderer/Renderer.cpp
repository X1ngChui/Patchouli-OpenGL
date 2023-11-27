#include "Renderer/Renderer.h"

namespace Pache
{
	Renderer::Scene* Renderer::scene = new Renderer::Scene;

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		scene->viewProjection = camera.getViewProjection();
	}

	void Renderer::endScence()
	{
	}

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->uploadUniform("viewProjection", scene->viewProjection);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}