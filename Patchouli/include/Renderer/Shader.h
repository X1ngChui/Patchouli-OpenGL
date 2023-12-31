#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Pache
{
	class Shader : public RefCounted
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		virtual Identifier getName() const = 0;

		virtual void set(Identifier name, int value) const = 0;
		virtual void set(Identifier name, const glm::vec3& vector) const = 0;
		virtual void set(Identifier name, const glm::vec4& vector) const = 0;
		virtual void set(Identifier name, const glm::mat4& matrix) const = 0;

		static Ref<Shader> create(Identifier name, const std::string& vertexSource, const std::string& fragmentSource);
		static Ref<Shader> create(const std::filesystem::path& path);
	};

	class ShaderLibruary
	{
	public:
		void add(const Ref<Shader>& shader);
		void add(Identifier name, const Ref<Shader>& shader);
		Ref<Shader> load(const std::filesystem::path& path);
		Ref<Shader> load(Identifier name, const std::filesystem::path& path);

		Ref<Shader> get(Identifier name) const;

		bool exists(Identifier name) const;
	private:
		std::unordered_map<Identifier, Ref<Shader>> shaders;
	};
}