#include "Renderer/Renderer2D.h"

namespace Pache
{
	struct Renderer2DData
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
		Ref<Texture2D> white;
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
			{ Pache::BufferElement::Float3, LITERAL_IDENTIFIER("a_position") },
			{ Pache::BufferElement::Float2, LITERAL_IDENTIFIER("a_textureCoord") }
		});
		data->vertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		auto indexBuffer = Pache::IndexBuffer::create(indices, sizeof(indices));
		data->vertexArray->setIndexBuffer(indexBuffer);

		data->white = Pache::Texture2D::create(1, 1);
		uint32_t white = 0xffffffff;
		data->white->setData(&white, sizeof(uint32_t));

		data->shader = Pache::Shader::create("assets/shaders/genericShader.glsl");
		data->shader->bind();
		data->shader->set(LITERAL_IDENTIFIER("u_texture"), 0);
	}

	void Renderer2D::shutdown()
	{
		delete data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		data->shader->bind();
		data->shader->set(LITERAL_IDENTIFIER("u_viewProjection"), camera.getViewProjection());
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
		data->shader->bind();
		data->shader->set(LITERAL_IDENTIFIER("u_color"), color);
		data->white->bind(0);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data->shader->set(LITERAL_IDENTIFIER("u_transform"), translation);

		data->vertexArray->bind();
		RenderCommand::drawIndexed(data->vertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		data->shader->bind();
		texture->bind(0);

		data->shader->set(LITERAL_IDENTIFIER("u_color"), glm::vec4(1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data->shader->set(LITERAL_IDENTIFIER("u_transform"), transform);

		data->vertexArray->bind();
		RenderCommand::drawIndexed(data->vertexArray);
	}
}