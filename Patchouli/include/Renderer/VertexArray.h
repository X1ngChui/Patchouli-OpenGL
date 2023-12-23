#pragma once

namespace Pache
{
	class VertexArray : public RefCounted
	{
	public:
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const IndexBuffer& getIndexBuffer() = 0;

		virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		static Ref<VertexArray> create();
	};
}