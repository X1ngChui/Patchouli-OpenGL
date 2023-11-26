#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "glad/glad.h"

namespace Pache
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size)
		: count(size / sizeof(uint32_t))
	{
		glCreateBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &indexBuffer);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}