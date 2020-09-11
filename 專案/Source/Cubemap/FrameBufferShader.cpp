#include "FrameBufferShader.h"
#include "ResourcePath.h"


FrameBufferShader::FrameBufferShader()
{
}


FrameBufferShader::~FrameBufferShader()
{
}

bool FrameBufferShader::Init(BufferType t)
{
	if (!ShaderObject::Init())
	{
		return false;
	}
	if (t == SHAOW_MAP)
		InitShadowMap();
	else if (t == COLOR_BUFFER)
		InitFrameBuffer();
	InitVertices();

	return true;
}



void FrameBufferShader::InitFrameBuffer()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &bufferTexture);
	glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 800, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTexture, 0);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenRenderbuffers(1, &rbo);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	 // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferShader::InitShadowMap()
{
	// 產生depth buffer
	glGenFramebuffers(1, &fbo);

	// 產生depth texture
	glGenTextures(1, &bufferTexture);
	glBindTexture(GL_TEXTURE_2D, bufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// 將depth textuer當作貞緩衝的緩衝
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferShader::InitVertices()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

void FrameBufferShader::RenderResult()
{
	glBindVertexArray(vao);
	//glBindTexture(GL_TEXTURE_2D, bufferTexture);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



void FrameBufferShader::genShadowMapBegin()
{
	// 1. 首?渲染深度??
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void FrameBufferShader::genShadowMapEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferShader::setupShadowMap()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBufferShader::setUniformVec2(string target, const glm::vec2 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform2fv(loc, 1, glm::value_ptr(value));
}
void FrameBufferShader::setUniformVec3(string target, const glm::vec3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform3fv(loc, 1, glm::value_ptr(value));
}
void FrameBufferShader::setUniformVec4(string target, const glm::vec4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform4fv(loc, 1, glm::value_ptr(value));
}
void FrameBufferShader::setUniformMat3(string target, const glm::mat3 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void FrameBufferShader::setUniformMat4(string target, const glm::mat4 &value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
void FrameBufferShader::setUniformInt(string  target, int value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform1i(loc, value);
}
void FrameBufferShader::setUniformFloat(string  target, float value)
{
	GLint loc = glGetUniformLocation(program, target.c_str());
	if (loc < 0)
	{
		cout << "Fail to set uniform variable " << target << endl;
		return;
	}
	glUniform1f(loc, value);
}