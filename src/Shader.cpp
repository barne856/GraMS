/*
 * Copyright (c) 2018 Brendan Barnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "Shader.hpp"

Shader::Shader(std::string& filepath)
    : mID(0)
{
    Shader::ShaderSource source = ParseShader(filepath);
    CompileShader(source);
}

Shader::~Shader()
{

}

Shader::ShaderSource Shader::ParseShader(std::string& filepath)
{
    std::ifstream stream(filepath);

	if (!stream.is_open())
	{
		std::cout << "Failed to open shader file!" << std::endl;
	}

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

void Shader::CompileShader(Shader::ShaderSource shaderSource)
{
    GLCall(unsigned int vs = glCreateShader(GL_VERTEX_SHADER));
	const char* vsSrc = shaderSource.VertexSource.c_str();
	GLCall(glShaderSource(vs, 1, &vsSrc, nullptr));
	GLCall(glCompileShader(vs));

	int result;
	GLCall(glGetShaderiv(vs, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(vs, length, &length, message));
		std::cout << "Failed to compile vertex shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(vs));
	}

    GLCall(unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER));
	const char* fsSrc = shaderSource.FragmentSource.c_str();
	GLCall(glShaderSource(fs, 1, &fsSrc, nullptr));
	GLCall(glCompileShader(fs));

	GLCall(glGetShaderiv(fs, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(fs, length, &length, message));
		std::cout << "Failed to compile fragment shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(fs));
	}

    GLCall(unsigned int program = glCreateProgram());

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	mID = program;
}

void Shader::Bind()
{
	GLCall(glUseProgram(mID));
}