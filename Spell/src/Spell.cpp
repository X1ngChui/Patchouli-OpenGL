#include "Spell.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Core/EntryPoint.h"

namespace Spell
{
	ExampleLayer::ExampleLayer()
		: Layer("Example Layer"), cameraController(1280.0f / 720.0f, true)
	{
		vertexArray = Pache::VertexArray::create();

		float vertices[] =
		{
			-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,   0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,   0.5f, 0.0f, 0.0f, 1.0f
		};

		vertexBuffer = Pache::VertexBuffer::create(vertices, sizeof(vertices));

		Pache::BufferLayout layout = {
			{ Pache::BufferElement::Float3, "a_position" },
			{ Pache::BufferElement::Float2, "a_texCoord" }
		};

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
		indexBuffer = Pache::IndexBuffer::create(indices, sizeof(indices));
		vertexArray->setIndexBuffer(indexBuffer);
		
		auto shader = shaderLibruary.load("assets/shaders/shader.glsl");

		texture = Pache::Texture2D::create("assets/textures/hikari.png");

		shader->bind();
		((Pache::OpenGLShader*)shader.get())->uploadUniform("u_texCoord", 0);
	}

	void ExampleLayer::onUpdate(Pache::Timestep timestep)
	{
		cameraController.onUpdate(timestep);

		Pache::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Pache::RenderCommand::clear();

		Pache::Renderer::beginScene(cameraController.getCamera());

		auto shader = shaderLibruary.get("shader");
		shader->bind();
		
		/*
		std::dynamic_pointer_cast<Pache::OpenGLShader>(shader)->uploadUniform("color", color);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 8; x++)
		{ 
			for (int y = 0; y < 8; y++)
			{
				glm::vec3 position = { 0.12f * x, 0.12f * y, 0.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				Pache::Renderer::submit(shader, vertexArray, transform);
			}
		}
		*/
		texture->bind(0);
		Pache::Renderer::submit(shader, vertexArray);
		Pache::Renderer::endScene();
	}

	void ExampleLayer::onImGuiRender()
	{
		// ImGui::Begin("settings");
		// ImGui::ColorEdit3("color", glm::value_ptr(color));
		// ImGui::End();
	}

	void ExampleLayer::onEvent(Pache::Event& evt)
	{
		Pache::Log::clientInfo(evt);
		cameraController.onEvent(evt);
	}
}