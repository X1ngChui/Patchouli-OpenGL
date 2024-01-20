#include "Renderer/Renderer2D.h"

namespace Pache
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoord;
		float textureIndex;
		float tilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 8192;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;


		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;
		Ref<Texture2D> white;

		uint32_t indexCount = 0;
		QuadVertex* vertexBasePtr = nullptr;
		QuadVertex* vertexBufferPtr = nullptr;
		std::array<Ref<Texture2D>, 32> textureSlots;
		uint32_t textureSlotIndex = 1;					// 0 is for white
	};

	static Renderer2DData data;

	void Renderer2D::init()
	{
		data.vertexArray = Pache::VertexArray::create();

		data.vertexBuffer = Pache::VertexBuffer::create(data.maxVertices * sizeof(QuadVertex));

		data.vertexBuffer->setLayout({
			{ Pache::BufferElement::ShaderDataType::Float3, LITERAL_IDENTIFIER("a_position") },
			{ Pache::BufferElement::ShaderDataType::Float4, LITERAL_IDENTIFIER("a_color") },
			{ Pache::BufferElement::ShaderDataType::Float2, LITERAL_IDENTIFIER("a_textureCoord") },
			{ Pache::BufferElement::ShaderDataType::Float,  LITERAL_IDENTIFIER("a_textureIndex") },
			{ Pache::BufferElement::ShaderDataType::Float,  LITERAL_IDENTIFIER("a_tilingFactor") }
		});
		data.vertexArray->addVertexBuffer(data.vertexBuffer);
		data.vertexBasePtr = new QuadVertex[data.maxVertices];


		uint32_t* quadIndices = new uint32_t[data.maxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.maxIndices; i += 6)
		{
			glm::u32vec4 index1 = glm::u32vec4(0, 1, 2, 2) + glm::u32vec4(offset);
			glm::u32vec2 index2 = glm::u32vec2(3, 0) + glm::u32vec2(offset);

			std::memcpy(&quadIndices[i], glm::value_ptr(index1), sizeof(glm::vec4));
			std::memcpy(&quadIndices[i + 4], glm::value_ptr(index2), sizeof(glm::vec2));
			
			offset += 4;
		}
		auto indexBuffer = Pache::IndexBuffer::create(quadIndices, data.maxIndices * sizeof(uint32_t));
		data.vertexArray->setIndexBuffer(indexBuffer);
		delete[] quadIndices;

		data.white = Pache::Texture2D::create(1, 1);
		uint32_t white = 0xffffffff;
		data.white->setData(&white, sizeof(uint32_t));

		uint32_t samplers[data.maxTextureSlots];
		for (uint32_t i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}

		data.shader = Pache::Shader::create("assets/shaders/genericShader.glsl");
		data.shader->bind();
		data.shader->setIntArray(LITERAL_IDENTIFIER("u_textures"), (int*)samplers, data.maxTextureSlots);

		data.textureSlots[0] = data.white;
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		data.shader->bind();
		data.shader->setMat4(LITERAL_IDENTIFIER("u_viewProjection"), camera.getViewProjection());

		data.indexCount = 0;
		data.textureSlotIndex = 1;
		data.vertexBufferPtr = data.vertexBasePtr;
	}

	void Renderer2D::endScene()
	{
		uint32_t size = (uint8_t*)data.vertexBufferPtr - (uint8_t*)data.vertexBasePtr;
		data.vertexBuffer->setData(data.vertexBasePtr, size);
		flush();
	}

	void Renderer2D::flush()
	{
		for (uint32_t i = 0; i < data.textureSlotIndex; i++)
		{
			data.textureSlots[i]->bind(i);
		}

		RenderCommand::drawIndexed(data.vertexArray, data.indexCount);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		data.vertexBufferPtr->position = position;
		data.vertexBufferPtr->color = color;
		data.vertexBufferPtr->textureCoord = { 0.0f, 0.0f };
		data.vertexBufferPtr->textureIndex = 0;
		data.vertexBufferPtr->tilingFactor = 1.0f;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(size.x, 0.0f, 0.0f);
		data.vertexBufferPtr->color = color;
		data.vertexBufferPtr->textureCoord = { 1.0f, 0.0f };
		data.vertexBufferPtr->textureIndex = 0;
		data.vertexBufferPtr->tilingFactor = 1.0f;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(size.x, size.y, 0.0f);
		data.vertexBufferPtr->color = color;
		data.vertexBufferPtr->textureCoord = { 1.0f, 1.0f };
		data.vertexBufferPtr->textureIndex = 0;
		data.vertexBufferPtr->tilingFactor = 1.0f;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(0.0f, size.y, 0.0f);
		data.vertexBufferPtr->color = color;
		data.vertexBufferPtr->textureCoord = { 0.0f, 1.0f };
		data.vertexBufferPtr->textureIndex = 0;
		data.vertexBufferPtr->tilingFactor = 1.0f;
		data.vertexBufferPtr++;
		
		data.indexCount += 6;

		/*data.shader->set(LITERAL_IDENTIFIER("u_color"), color);
		data.shader->set(LITERAL_IDENTIFIER("u_tilingFactor"), 1.0f);
		data.white->bind(0);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.shader->set(LITERAL_IDENTIFIER("u_transform"), translation);

		data.vertexArray->bind();
		RenderCommand::drawIndexed(data.vertexArray);*/
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < data.textureSlotIndex; i++)
		{
			if (*data.textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)data.textureSlotIndex;
			data.textureSlots[data.textureSlotIndex] = texture;
			data.textureSlotIndex++;
		}

		data.vertexBufferPtr->position = position;
		data.vertexBufferPtr->color = tintColor;
		data.vertexBufferPtr->textureCoord = { 0.0f, 0.0f };
		data.vertexBufferPtr->textureIndex = textureIndex;
		data.vertexBufferPtr->tilingFactor = tilingFactor;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(size.x, 0.0f, 0.0f);
		data.vertexBufferPtr->color = tintColor;
		data.vertexBufferPtr->textureCoord = { 1.0f, 0.0f };
		data.vertexBufferPtr->textureIndex = textureIndex;
		data.vertexBufferPtr->tilingFactor = tilingFactor;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(size.x, size.y, 0.0f);
		data.vertexBufferPtr->color = tintColor;
		data.vertexBufferPtr->textureCoord = { 1.0f, 1.0f };
		data.vertexBufferPtr->textureIndex = textureIndex;
		data.vertexBufferPtr->tilingFactor = tilingFactor;
		data.vertexBufferPtr++;

		data.vertexBufferPtr->position = position + glm::vec3(0.0f, size.y, 0.0f);
		data.vertexBufferPtr->color = tintColor;
		data.vertexBufferPtr->textureCoord = { 0.0f, 1.0f };
		data.vertexBufferPtr->textureIndex = textureIndex;
		data.vertexBufferPtr->tilingFactor = tilingFactor;
		data.vertexBufferPtr++;

		data.indexCount += 6;

		/*data.shader->bind();
		texture->bind(0);

		data.shader->set(LITERAL_IDENTIFIER("u_color"), tintColor);
		data.shader->set(LITERAL_IDENTIFIER("u_tilingFactor"), tilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		data.shader->set(LITERAL_IDENTIFIER("u_transform"), transform);

		data.vertexArray->bind();
		RenderCommand::drawIndexed(data.vertexArray);*/
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
		const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const glm::vec4& color)
	{
		data.shader->setFloat4(LITERAL_IDENTIFIER("u_color"), color);
		data.shader->setFloat(LITERAL_IDENTIFIER("u_tilingFactor"), 1.0f);
		data.white->bind(0);

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.shader->setMat4(LITERAL_IDENTIFIER("u_transform"), translation);

		data.vertexArray->bind();
		RenderCommand::drawIndexed(data.vertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		data.shader->bind();
		texture->bind(0);

		data.shader->setFloat4(LITERAL_IDENTIFIER("u_color"), tintColor);
		data.shader->setFloat(LITERAL_IDENTIFIER("u_tilingFactor"), tilingFactor);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		data.shader->setMat4(LITERAL_IDENTIFIER("u_transform"), transform);

		data.vertexArray->bind();
		RenderCommand::drawIndexed(data.vertexArray);
	}
}