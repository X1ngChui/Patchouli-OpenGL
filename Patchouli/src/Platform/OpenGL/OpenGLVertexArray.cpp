#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Pache
{
	static GLenum toOpenGLBaseType(BufferElement::ShaderDataType type)
	{
		switch (type)
		{
		case BufferElement::ShaderDataType::Bool:
			return GL_BOOL;
		case BufferElement::ShaderDataType::Int:
			return GL_INT;
		case BufferElement::ShaderDataType::Int2:
			return GL_INT;
		case BufferElement::ShaderDataType::Int3:
			return GL_INT;
		case BufferElement::ShaderDataType::Int4:
			return GL_INT;
		case BufferElement::ShaderDataType::Float:
			return GL_FLOAT;
		case BufferElement::ShaderDataType::Float2:
			return GL_FLOAT;
		case BufferElement::ShaderDataType::Float3:
			return GL_FLOAT;
		case BufferElement::ShaderDataType::Float4:
			return GL_FLOAT;
		case BufferElement::ShaderDataType::Mat3:
			return GL_FLOAT;
		case BufferElement::ShaderDataType::Mat4:
			return GL_FLOAT;
		default:
			Log::coreAssert(false, "Unknown shader data type");
		}
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &vertexArray);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(vertexArray);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		Log::coreAssert(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout");

		glBindVertexArray(vertexArray);
		vertexBuffer->bind();

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->getLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				BufferElement::getComponents(element.getType()),
				toOpenGLBaseType(element.getType()),
				element.getNormalized() ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.getOffset()
			);
			index++;
		}
		vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(vertexArray);
		indexBuffer->bind();

		this->indexBuffer = indexBuffer;
	}
}