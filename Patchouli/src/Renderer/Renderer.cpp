#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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

	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniform("viewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniform("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}