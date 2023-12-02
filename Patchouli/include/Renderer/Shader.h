#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Identifier.h"

namespace Pache
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void bind() const;
		void unbind() const;

		void uploadUniform(const Identifier name, const glm::mat4& matrix) const;
	private:
		uint32_t program;
	};
}