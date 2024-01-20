#pragma once

namespace Pache
{
	class BufferElement
	{
	public:
		enum class ShaderDataType
		{
			None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4
		};

		BufferElement(ShaderDataType type, Identifier name, bool normalized = false);

		ShaderDataType getType() const { return type; }
		Identifier getName() const { return name; }
		bool getNormalized() const { return normalized; }
		uint32_t getSize() const { return size; }
		uintptr_t getOffset() const { return offset; }

		static uint32_t getComponents(ShaderDataType type);
		static uint32_t toSize(ShaderDataType type);
	private:
		friend class BufferLayout;
		ShaderDataType type;
		Identifier name;
		bool normalized;
		uint32_t size;
		uintptr_t offset;
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		const std::vector<BufferElement>& getElements() const { return elements; }
		uint32_t getStride() const { return stride; }

		auto begin() const { return elements.begin(); }
		auto end() const { return elements.end(); }
		auto cbegin() const { return elements.cbegin(); }
		auto cend() const { return elements.cend(); }
	private:
		std::vector<BufferElement> elements;
		uint32_t stride;
	};

	class VertexBuffer : public RefCounted
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& getLayout() const = 0;
		virtual void setData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> create(uint32_t size);
		static Ref<VertexBuffer> create(float* vertices, uint32_t size);
	};
}