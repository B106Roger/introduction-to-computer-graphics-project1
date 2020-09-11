#pragma once
#include <Common.h>
#include <ResourcePath.h>
#include "DrawModelShader.h"
#include <iostream>
using namespace std;
class SpotLight
{
public:
	static const float SpotLight::vertices[108];
	SpotLight();
	bool Init();
	~SpotLight();
	void draw();
	void generateShaowMapBegin();
	void generateShaowMapEnd();
	void setupShadowMap();
	void setPosition(glm::vec3 val) { position = val; dirty = true; }
	void setView(glm::mat4 val) { view = val; dirty = true; }
	void setProjection(glm::mat4 val) { projection = val; dirty = true; }
	void setScale(float val) { scale = val; dirty = true; }
	GLuint getTextureShadowMap() { return depthMap; };
	glm::vec3 getPosition() { return position; }
	glm::mat4 getLightSpacePerspectMatrix();
	glm::mat4 getLightSpaceOthoMatrix();

	// SpotLight
	glm::vec3 spotDirection;
	// cosine value
	float innerCutOff;
	// cosine value
	float outerCutOff;

	// 酚把计
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// 癐搭把计
	float constant;
	float linear;
	float quardratic;

	// 睦戈方
	void release();
private:
	// 膀娄把计
	glm::vec3 position;
	float scale;

	// opengl 把计
	GLuint vao;
	GLuint vbo;
	DrawModelShader shader;

	// view
	glm::mat4 view;
	// projection
	glm::mat4 projection;
private:
	bool dirty;
	// Shadow
	const GLuint SHADOW_WIDTH = 2048 * 2;
	const GLuint SHADOW_HEIGHT = 2048 * 2;
	const int SCR_WIDTH = 800;
	const int SCR_HEIGHT = 600;
	void initShadowMap();
	GLuint depthMapFBO;
	GLuint depthMap;
};

