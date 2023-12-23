#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Pache
{
	Ref<Shader> Shader::create(Identifier name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLShader>(name, vertexSource, fragmentSource);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::filesystem::path& path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::None:
			return nullptr;
		case RendererAPI::OpenGL:
			return makeIntrusive<OpenGLShader>(path);
		default:
			Log::coreAssert(false, "Unknown renderer API");
		}
		return nullptr;
	}

	void ShaderLibruary::add(const Ref<Shader>& shader)
	{
		Identifier name = shader->getName();
		Log::coreAssert(!exists(name), "Shader already exists.");
		shaders[name] = shader;
	}

	void ShaderLibruary::add(Identifier name, const Ref<Shader>& shader)
	{
		Log::coreAssert(!exists(name), "Shader already exists.");
		shaders[name] = shader;
	}

	Ref<Shader> ShaderLibruary::load(const std::filesystem::path& path)
	{
		Ref<Shader> shader = Shader::create(path);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibruary::load(Identifier name, const std::filesystem::path& path)
	{
		Ref<Shader> shader = Shader::create(path);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibruary::get(Identifier name) const
	{
		auto it = shaders.find(name);
		Log::coreAssert(it != shaders.end(), "The shader not found.");
		return it->second;
	}

	bool ShaderLibruary::exists(Identifier name) const
	{
		return shaders.find(name) != shaders.end();
	}
}