#include "Light.h"

const float SpotLight::vertices[108] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f
};


SpotLight::SpotLight() :
	position(5.f, 0.f, 0.f), scale(0.2f),
	ambient(0.1), diffuse(0.5f, 0.5f, 0.5f), specular(1.0f, 1.0f, 1.0f),
	constant(1.f), linear(0.019), quardratic(0.0022), dirty(true),
	spotDirection(0.f ,-1.f, 1.f), innerCutOff(cos(glm::radians(12.5f))), outerCutOff(glm::radians(17.5f))
{
	
}

bool SpotLight::Init()
{
	shader.Init();
	shader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "light.vs.glsl");
	shader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "light.fs.glsl");
	shader.Finalize();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// 初始化vertex
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpotLight::vertices), SpotLight::vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	this->initShadowMap();
	return true;
}

SpotLight::~SpotLight()
{
}

void SpotLight::draw()
{
	shader.Enable();
	glBindVertexArray(vao);
	if (dirty)
	{
		glm::mat4 model(1.f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));

		shader.setUniformMat4("model", model);
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);

	}
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void SpotLight::release()
{
	// delete ShaderProgram
}
void SpotLight::initShadowMap()
{
	// 產生depth buffer
	glGenFramebuffers(1, &depthMapFBO);

	// 產生depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// 將depth textuer當作貞緩衝的緩衝
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SpotLight::generateShaowMapBegin()
{
	// 1. 首选渲染深度贴图
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}
void SpotLight::generateShaowMapEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SpotLight::setupShadowMap()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


glm::mat4 SpotLight::getLightSpacePerspectMatrix()
{
	return glm::perspective(glm::radians(120.f), 1.f, 0.1f, 100.f) * glm::lookAt(getPosition(), glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
}
glm::mat4 SpotLight::getLightSpaceOthoMatrix()
{
	return  glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.f) * glm::lookAt(getPosition(), glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
}