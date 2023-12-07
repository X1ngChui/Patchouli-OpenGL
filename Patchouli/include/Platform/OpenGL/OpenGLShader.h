#pragma once
#include "Renderer/Shader.h"
#include "glm/glm.hpp"

namespace Pache
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		void uploadUniform(const Identifier name, int value) const;

		void uploadUniform(const Identifier name, float value) const;
		void uploadUniform(const Identifier name, const glm::vec2& vector) const;
		void uploadUniform(const Identifier name, const glm::vec3& vector) const;
		void uploadUniform(const Identifier name, const glm::vec4& vector) const;

		void uploadUniform(const Identifier name, const glm::mat3& matrix) const;
		void uploadUniform(const Identifier name, const glm::mat4& matrix) const;
	private:
		uint32_t program;
	};
}