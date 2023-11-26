#pragma once
#include "Renderer/VertexBuffer.h"

namespace Pache
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout& layout) override { this->layout = layout; }
		virtual const BufferLayout& getLayout() const override { return layout; }
	private:
		uint32_t vertexBuffer;
		BufferLayout layout;
	};
}