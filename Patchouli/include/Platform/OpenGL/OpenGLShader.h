#pragma once
#include "Renderer/Shader.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Pache
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(Identifier name, const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(const std::filesystem::path& path);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual Identifier getName() const override { return name; }

		virtual void setInt(Identifier name, int value) const override;
		virtual void setIntArray(Identifier name, int* values, uint32_t count) const override;
		virtual void setFloat(Identifier name, float value) const override;
		virtual void setFloat3(Identifier name, const glm::vec3& vector) const override;
		virtual void setFloat4(Identifier name, const glm::vec4& vector) const override;
		virtual void setMat4(Identifier name, const glm::mat4& matrix) const override;

		void uploadUniformInt(Identifier name, int value) const;
		void uploadUniformIntArray(Identifier name, int* values, uint32_t count) const;
		void uploadUniformFloat(Identifier name, float value) const;
		void uploadUniformFloat2(Identifier name, const glm::vec2& vector) const;
		void uploadUniformFloat3(Identifier name, const glm::vec3& vector) const;
		void uploadUniformFloat4(Identifier name, const glm::vec4& vector) const;

		void uploadUniformMat3(Identifier name, const glm::mat3& matrix) const;
		void uploadUniformMat4(Identifier name, const glm::mat4& matrix) const;
	private:
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		std::string readFile(const std::filesystem::path& path) const;
	private:
		Identifier name;
		uint32_t program;
	};
}