#pragma once
#include "Renderer/VertexArray.h"

namespace Pache
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
	private:
		uint32_t vertexArray;
		std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
		std::shared_ptr<IndexBuffer> indexBuffer;
	};
}