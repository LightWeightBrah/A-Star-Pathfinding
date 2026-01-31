#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: filepath(filepath), boneId(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	boneId = CreateProgram(source.vertexSource, source.fragmentSource);
	Bind();
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(boneId));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return  { ss[0].str(), ss[1].str() };

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int boneId = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(boneId, 1, &src, nullptr);
	glCompileShader(boneId);

	int success;
	glGetShaderiv(boneId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(boneId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(boneId, length, &length, message);
		std::cout << "ERROR: COULDN'T COMPILE " <<
			(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER: " << message << std::endl;
		glDeleteShader(boneId);
		return 0;
	}

	return boneId;
}

unsigned int Shader::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "ERROR: LINKING SHADERS TO PROGRAM FAILED: " << message << std::endl;
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(boneId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) const
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) const
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(boneId, name.c_str()));
	if (location == -1)
		std::cout << "UNIFORM: " << name << " DOESN'T EXIST" << std::endl;
	
	uniformLocationCache[name] = location;
	return location;
}
