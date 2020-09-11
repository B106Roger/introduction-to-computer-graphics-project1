#include "Shader.h"



Shader::Shader()
{
}


Shader::~Shader()
{
	for (std::vector<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	if (program != 0)
	{
		glDeleteProgram(program);
		program = 0;
	}

}

bool Shader::Init()
{
	program = glCreateProgram();
	if (program == 0)
	{
		puts("Create program error\n");
		return false;
	}

	return true;
}

bool Shader::AddShader(GLenum shaderType, std::string fileName)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		printf("Create shader error: %d\n", shaderType);
		return false;
	}

	shaderList.push_back(shader);

	char **shaderSource = Common::LoadShaderSource((fileName).c_str());

	glShaderSource(shader, 1, shaderSource, NULL);
	Common::FreeShaderSource(shaderSource);

	glCompileShader(shader);
	bool isCompiled = Common::CheckShaderCompiled(shader);
	if (!isCompiled)
	{
		return false;
	}

	glAttachShader(program, shader);

	return Common::CheckGLError();
}

bool Shader::Finalize()
{
	glLinkProgram(program);
	bool isLinked = Common::CheckProgramLinked(program);

	for (std::vector<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	return isLinked;
}

void Shader::Enable()
{
	glUseProgram(program);
}

void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::setUniformVec2(string target, const glm::vec2 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform2fv(loc, 1, glm::value_ptr(value));
}
void Shader::setUniformVec3(string target, const glm::vec3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
void Shader::setUniformVec4(string target, const glm::vec4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform4fv(loc, 1, glm::value_ptr(value));
}
void Shader::setUniformMat3(string target, const glm::mat3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniformMat4(string target, const glm::mat4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniformInt(string  target, int value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform1i(loc, value);
}
void Shader::setUniformFloat(string  target, float value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform1f(loc, value);
}