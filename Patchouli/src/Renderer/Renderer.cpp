#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pache
{
	Renderer::Scene* Renderer::scene = new Renderer::Scene;

	void Renderer::init()
	{
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		scene->viewProjection = camera.getViewProjection();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		((OpenGLShader*)shader.get())->uploadUniform("viewProjection", scene->viewProjection);
		((OpenGLShader*)shader.get())->uploadUniform("transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}