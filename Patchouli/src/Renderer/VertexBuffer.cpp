#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Pache
{
	BufferElement::BufferElement(ShaderDataType type, Identifier name, bool normalized)
		: type(type), name(name), normalized(normalized), size(toSize(type)), offset(0)
	{
	}

	uint32_t BufferElement::getComponents(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Int4:
			return 4;
		case ShaderDataType::Float:
			return 1;
		case ShaderDataType::Float2:
			return 2;
		case ShaderDataType::Float3:
			return 3;
		case ShaderDataType::Float4:
			return 4;
		case ShaderDataType::Mat3:
			return 9;
		case ShaderDataType::Mat4:
			return 16;
		default:
			Log::coreAssert(false, "Unknown shader data type");
		}
		return 0;
	}

	uint32_t BufferElement::toSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 8;
		case ShaderDataType::Int3:
			return 12;
		case ShaderDataType::Int4:
			return 16;
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 8;
		case ShaderDataType::Float3:
			return 12;
		case ShaderDataType::Float4:
			return 16;
		case ShaderDataType::Mat3:
			return 36;
		case ShaderDataType::Mat4:
			return 64;
		default:
			Log::coreAssert(false, "Unknown shader data type");
		}
		return 0;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: elements(elements), stride(0)
	{
		for (BufferElement& element : this->elements)
		{
			element.offset = stride;
			stride += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLVertexBuffer>(vertices, size);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}
}