#pragma once

#include <ShaderObject.h>
#include <string>
using namespace std;
enum BufferType { SHAOW_MAP, COLOR_BUFFER};
class FrameBufferShader: 
	public ShaderObject
{
public:
	FrameBufferShader();
	~FrameBufferShader();

	bool Init(BufferType t = COLOR_BUFFER);
	
	void bindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
	void unbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	void RenderResult();

	void setUniformVec2(string target, const glm::vec2 &value);
	void setUniformVec3(string target, const glm::vec3 &value);
	void setUniformVec4(string target, const glm::vec4 &value);
	void setUniformMat3(string target, const glm::mat3 &value);
	void setUniformMat4(string target, const glm::mat4 &value);
	void setUniformInt(string  target, int value);
	void setUniformFloat(string  target, float value);

	GLuint getTextureId() { return bufferTexture; }

	void genShadowMapBegin();
	void genShadowMapEnd();
	void setupShadowMap();

	const GLuint SHADOW_WIDTH = 2048 * 4;
	const GLuint SHADOW_HEIGHT = 2048 * 4;
	const int SCR_WIDTH = 800;
	const int SCR_HEIGHT = 800;
private:
	void InitFrameBuffer();
	void InitShadowMap();
	void InitVertices();
	GLuint fbo;
	GLuint rbo;
	GLuint bufferTexture;

	GLuint vao;
};

