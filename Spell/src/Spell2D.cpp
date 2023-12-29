#include "Spell2D.h"
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Spell
{
	Spell2D::Spell2D()
		: Layer("Spell 2D"), cameraController(1280.0f / 720.0f)
	{
	}

	void Spell2D::onAttach()
	{
		vertexArray = Pache::VertexArray::create();

		float vertices[] =
		{
			-0.5f,  -0.5f,  0.0f,
			 0.5f,  -0.5f,  0.0f,
			 0.5f,   0.5f,  0.0f,
			-0.5f,   0.5f,  0.0f
		};

		auto vertexBuffer = Pache::VertexBuffer::create(vertices, sizeof(vertices));

		vertexBuffer->setLayout({
			{ Pache::BufferElement::Float3, "a_position" }
		});
		vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		auto indexBuffer = Pache::IndexBuffer::create(indices, sizeof(indices));
		vertexArray->setIndexBuffer(indexBuffer);

		shader = Pache::Shader::create("assets/shaders/flatColor.glsl");
	}

	void Spell2D::onDetach()
	{
	}

	void Spell2D::onUpdate(Pache::Timestep ts)
	{
		cameraController.onUpdate(ts);

		Pache::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Pache::RenderCommand::clear();

		Pache::Renderer::beginScene(cameraController.getCamera());
		shader->bind();
		((Pache::OpenGLShader*)shader.get())->uploadUniform("u_color", color);
		Pache::Renderer::submit(shader, vertexArray);
		Pache::Renderer::endScence();
	}

	void Spell2D::onImGuiRender()
	{
		ImGui::Begin("settings");
		ImGui::ColorEdit4("color", glm::value_ptr(color));
		ImGui::End();
	}

	void Spell2D::onEvent(Pache::Event& evt)
	{
		cameraController.onEvent(evt);
	}
}