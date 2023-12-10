#include "Platform/OpenGL/OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "glfw/glfw3.h"

namespace Pache
{
	static GLenum shaderTypeFromString(std::string& type)
	{
		std::transform(type.begin(), type.end(), type.begin(),
			[](unsigned char c)
			{
				return std::tolower(c);
			});
		
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		Log::coreError("Unknown shader type.");
		return 0;
	}

	OpenGLShader::OpenGLShader(Identifier name, const std::string& vertexSource, const std::string& fragmentSource)
		: name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::filesystem::path& path)
		: name(path.filename().stem().generic_string())
	{
		std::string src = readFile(path);

		std::regex pattern(R"#(\s*#type\s+(\w+)\s*\n((?:[^]*?)(?=#type)|[^]*$))#", std::regex_constants::icase);
		std::smatch matches;
		std::unordered_map<GLenum, std::string> shaderSources;

		auto srcBegin = src.cbegin();
		auto srcEnd = src.cend();

		while (std::regex_search(srcBegin, srcEnd, matches, pattern))
		{
			std::string shaderType = matches[1];
			std::string shaderCode = matches[2];

			// Store shader code in the map
			shaderSources[shaderTypeFromString(shaderType)] = shaderCode;

			// Update input iterator for the next iteration
			srcBegin = matches.suffix().first;
		}

		compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(program);
	}

	void OpenGLShader::bind() const
	{
		glUseProgram(program);
	}

	void OpenGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::uploadUniform(Identifier name, int value) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::uploadUniform(Identifier name, float value) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::uploadUniform(Identifier name, const glm::vec2& vector) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::uploadUniform(Identifier name, const glm::vec3& vector) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::uploadUniform(Identifier name, const glm::vec4& vector) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::uploadUniform(Identifier name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniform(Identifier name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(program, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		Log::coreAssert(shaderSources.size() <= 2, "Too many shaders.");

		// Get a program object.
		GLuint program = glCreateProgram();

		std::array<GLenum, 2> glShaders;

		size_t shaderIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& src = kv.second;

			// Create an empty shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			const GLchar* source = (const GLchar*)src.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the shader
			glCompileShader(shader);
			
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				Log::coreAssert(false, "Shader compilation failed: {0}", infoLog.data());
				return;
			}

			// Attach the shader to our program
			glAttachShader(program, shader);
			glShaders[shaderIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : glShaders)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			Log::coreAssert(false, "Shader program linking failed", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaders)
		{
			glDetachShader(program, id);
		}

		this->program = program;
	}

	std::string OpenGLShader::readFile(const std::filesystem::path& path) const
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		if (file)
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			file.close();

			return buffer.str();
		}

		Log::coreError("Cannot find the file at {}", path.string());
		return std::string();
	}
}