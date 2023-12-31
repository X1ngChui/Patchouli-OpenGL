#include "Renderer/Renderer2D.h"

namespace Pache
{
	struct Renderer2DData
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> colorShader;
		Ref<Shader> textureShader;
	};

	static Renderer2DData* data;

	void Renderer2D::init()
	{
		data = new Renderer2DData;
		data->vertexArray = Pache::VertexArray::create();

		float vertices[] =
		{
			-0.5f,  -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f,  -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,   0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,   0.5f,  0.0f, 0.0f, 1.0f
		};

		auto vertexBuffer = Pache::VertexBuffer::create(vertices, sizeof(vertices));

		vertexBuffer->setLayout({
			{ Pache::BufferElement::Float3, "a_position" },
			{ Pache::BufferElement::Float2, "a_textureCoord" }
		});
		data->vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		auto indexBuffer = Pache::IndexBuffer::create(indices, sizeof(indices));
		data->vertexArray->setIndexBuffer(indexBuffer);

		data->colorShader = Pache::Shader::create("assets/shaders/flatColor.glsl");
		data->textureShader = Pache::Shader::create("assets/shaders/texture.glsl");
		data->textureShader->bind();
		data->textureShader->set("u_texture", 0);
	}

	void Renderer2D::shutdown()
	{
		delete data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		data->colorShader->bind();
		data->colorShader->set("u_viewProjection", camera.getViewProjection());

		data->textureShader->bind();
		data->textureShader->set("u_viewProjection", camera.getViewProjection());
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		data->colorShader->bind();
		data->colorShader->set("u_color", color);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->colorShader->set("u_transform", translation);

		data->vertexArray->bind();
		RenderCommand::drawIndexed(data->vertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		data->textureShader->bind();
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->textureShader->set("u_transform", translation);

		texture->bind(0);

		data->vertexArray->bind();
		RenderCommand::drawIndexed(data->vertexArray);
	}
}