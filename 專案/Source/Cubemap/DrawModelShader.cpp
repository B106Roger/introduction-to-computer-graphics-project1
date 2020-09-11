#include "DrawModelShader.h"
#include "ResourcePath.h"


DrawModelShader::DrawModelShader()
{
}


DrawModelShader::~DrawModelShader()
{
}

bool DrawModelShader::Init()
{
	if (!ShaderObject::Init())
	{
		return false;
	}

	return true;
}

bool DrawModelShader::LoadTexture(string s)
{
	TextureData t = Common::Load_png(s.c_str());
	if (t.data == NULL)
	{
		cout << "LOAD IMAGE " << s << " FAIL" << endl;
		return false;
	}
	else
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		delete t.data;
		textures.push_back(texture);

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
}

bool DrawModelShader::InitUniformLocation()
{
	this->Enable();
	um4mvLocation = GetUniformLocation("um4mv");
	if (um4mvLocation == -1)
	{
		puts("Get uniform loaction error: um4mv");
		//return false;
	}

	um4mLocation = GetUniformLocation("um4m");
	if (um4mLocation == -1)
	{
		puts("Get uniform loaction error: um4m");
		//return false;
	}

	cameraPosLocation = GetUniformLocation("cameraPos");
	if (cameraPosLocation == -1)
	{
		puts("Get uniform loaction error: cameraPos");
		//return false;
	}

	um3nLocation = GetUniformLocation("um3n");
	if (um3nLocation == -1)
	{
		puts("Get uniform loaction error: um3n");
		//return false;
	}

	um4pLocation = GetUniformLocation("um4p");
	if (um4pLocation == -1)
	{
		puts("Get uniform loaction error: um4p");
		//return false;
	}

	
	this->Disable();
	return true;
}

void DrawModelShader::SetMMat(const glm::mat4& mat)
{
	glUniformMatrix4fv(um4mLocation, 1, GL_FALSE, glm::value_ptr(mat));
	// SetMVMat
	/*glm::mat4 mvMat = viewMat4 * mat;
	glUniformMatrix4fv(um4mvLocation, 1, GL_FALSE, glm::value_ptr(mvMat));*/
	setUniformMat4("um4v", viewMat4);
	// setUniformMat4("um4m", mat);
	// SetNormalMat
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(mat)));
	glUniformMatrix3fv(um3nLocation, 1, GL_FALSE, glm::value_ptr(normalMat));
}

//void DrawModelShader::SetMVMat(const glm::mat4& mat)
//{
//	glUniformMatrix4fv(um4mvLocation, 1, GL_FALSE, glm::value_ptr(mat));
//}

void DrawModelShader::SetPMat(const glm::mat4& mat)
{
	glUniformMatrix4fv(um4pLocation, 1, GL_FALSE, glm::value_ptr(mat));
}

//void DrawModelShader::SetNormalMat(const glm::mat3& mat)
//{
//	glUniformMatrix3fv(um3nLocation, 1, GL_FALSE, glm::value_ptr(mat));
//}

void DrawModelShader::SetCameraPos(const glm::vec3& vec)
{
	glUniform3fv(cameraPosLocation, 1, glm::value_ptr(vec));
}

void DrawModelShader::setUniformVec2(string target, const glm::vec2 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniform2fv(loc, 1, glm::value_ptr(value));
}
void DrawModelShader::setUniformVec3(string target, const glm::vec3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
void DrawModelShader::setUniformVec4(string target, const glm::vec4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniform4fv(loc, 1, glm::value_ptr(value));
}
void DrawModelShader::setUniformMat3(string target, const glm::mat3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void DrawModelShader::setUniformMat4(string target, const glm::mat4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void DrawModelShader::setUniformInt(string  target, int value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	if (target == "tex")
	{
		this;
		int a = 0;
	}
	glUniform1i(loc, value);
}
void DrawModelShader::setUniformFloat(string  target, float value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		Common::PrintGLError();
		return;
	}
	glUniform1f(loc, value);
}