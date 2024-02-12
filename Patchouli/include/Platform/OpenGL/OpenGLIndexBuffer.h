#pragma once
#include "Renderer/IndexBuffer.h"

namespace Pache
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const override { return count; }
	private:
		uint32_t count;
		uint32_t indexBuffer;
	};
}