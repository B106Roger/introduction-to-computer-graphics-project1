#pragma once

#include <Common.h>
#include <vector>
#include <iostream>
using namespace std;

class MyModel
{
public:
	MyModel();
	~MyModel();

	bool Init(std::string fileName, std::string matFileName = "");
	bool InitInstanced(std::string fileName, int maxInstanced, std::string matFileName = "");

	void Render();
	void setParentModel(glm::mat4 val) { parentModel = val; dirty = true; }
	void setCurrentModel(glm::mat4 val) { currentModel = val; dirty = true; }

	void RenderInstance(int count);
	void bufferInstanceModel();
	void setInstanceModel(glm::mat4 &model, int index);

	void SetCurrentCount(int count) { currentCount = glm::min(maxCount, count); }

	glm::mat4 getModelMatrix();
	glm::vec3 CenterPivot;
private:
	int indiceCount;
	GLuint vao;
	GLuint ebo;
	GLuint vboVertices, vboNormal, vboModel;
	glm::mat4 parentModel;
	glm::mat4 currentModel;
	glm::mat4 cache;
	bool dirty = true;

	glm::mat4 *ModelMatrix;
	glm::mat4 *FinalModelMatrix;
	int maxInstance;

	int maxCount;
	int currentCount;
};

