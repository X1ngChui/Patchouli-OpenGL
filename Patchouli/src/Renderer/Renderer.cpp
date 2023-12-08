#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Pache
{
	Renderer::Scene* Renderer::scene = new Renderer::Scene;

	void Renderer::init()
	{
		RenderCommand::init();
	}

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		scene->viewProjection = camera.getViewProjection();
	}

	void Renderer::endScence()
	{
	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniform("viewProjection", scene->viewProjection);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniform("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}