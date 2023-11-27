#include "Spell.h"

namespace Spell
{
	ExampleLayer::ExampleLayer()
		: Layer("Example Layer"), camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		vertexArray = std::shared_ptr<Pache::VertexArray>(Pache::VertexArray::create());

		float vertices[] =
		{
			-0.5f,  -0.29f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f,  -0.29f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,   0.58f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		vertexBuffer = std::shared_ptr<Pache::VertexBuffer>(Pache::VertexBuffer::create(vertices, sizeof(vertices)));

		Pache::BufferLayout layout = {
			{ Pache::BufferElement::Float3, "position" },
			{ Pache::BufferElement::Float4, "color"}
		};

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		indexBuffer = std::shared_ptr<Pache::IndexBuffer>(Pache::IndexBuffer::create(indices, sizeof(indices)));
		vertexArray->setIndexBuffer(indexBuffer);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;

			uniform mat4 viewProjection;

			out vec3 v_position;
			out vec4 v_color;
			
			void main()
			{
				v_position = position;
				v_color = color;
				gl_Position = viewProjection * vec4(position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;

			in vec3 v_position;
			in vec4 v_color;
			
			void main()
			{
				color = vec4(v_position * 0.5 + 0.5, 1.0);
				color = v_color;
			}
		)";

		shader = std::make_unique<Pache::Shader>(vertexSource, fragmentSource);
	}

	void ExampleLayer::onUpdate()
	{
		camera.setRotation(camera.getRotation() + 0.01f);

		Pache::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Pache::RenderCommand::clear();

		Pache::Renderer::beginScene(camera);
		Pache::Renderer::submit(shader, vertexArray);
		Pache::Renderer::endScence();
	}
}