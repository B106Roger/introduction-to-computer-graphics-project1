#include "MyModel.h"

MyModel::MyModel():
	ModelMatrix(NULL)
{
}


MyModel::~MyModel()
{
}

bool MyModel::Init(std::string fileName, std::string matFileName)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	const char * matFilePtr = NULL;
	if (matFileName != "")
		matFilePtr = matFileName.c_str();

	bool ret = tinyobj::LoadObj(shapes, materials, err, fileName.c_str(), matFilePtr);
	if (!ret)
	{
		printf("Load Models Fail! Please check the solution path");
		return false;
	}
	
	printf("Load Models Success ! Shapes size %d Maerial size %d\n", shapes.size(), materials.size());

	unsigned int accumulateDataSize = 0, accumulateIndicesSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		for (unsigned int &index: shapes[i].mesh.indices) 
		{
			index += accumulateDataSize / 3;
		}
		accumulateDataSize += shapes[i].mesh.positions.size();
		accumulateIndicesSize += shapes[i].mesh.indices.size();
	}
	

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, accumulateDataSize * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	unsigned int previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.positions.size() * sizeof(float), &(shapes[i].mesh.positions[0]));
		previousDataSize += shapes[i].mesh.positions.size() * sizeof(float);
	}
	assert(previousDataSize == accumulateDataSize * sizeof(float));
	Common::PrintGLError();
	
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, accumulateDataSize * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.normals.size() * sizeof(float), &(shapes[i].mesh.normals[0]));
		previousDataSize += shapes[i].mesh.normals.size() * sizeof(float);
	}
	assert(previousDataSize == accumulateDataSize * sizeof(float));
	Common::PrintGLError();
	
	glEnableVertexAttribArray(1);

	GLuint colorVbo;
	glm::vec3 pos[5] = {
		glm::vec3(0.f),
		glm::vec3(3.0, 0.0, -6.0),
		glm::vec3(-3.0, 0.0, -6.0),
		glm::vec3(-5.0, 0.0, -12.0),
		glm::vec3(5.0, 0.0, -12.0)
	};

	maxCount = sizeof(pos) / sizeof(glm::vec3);
	currentCount = 1;

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribDivisor(2, 1);
	Common::PrintGLError();
	glEnableVertexAttribArray(2);
	

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, accumulateIndicesSize * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
	previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.indices.size() * sizeof(unsigned int), &(shapes[i].mesh.indices[0]));
		previousDataSize += shapes[i].mesh.indices.size() * sizeof(unsigned int);
	}
	assert(previousDataSize == accumulateIndicesSize * sizeof(unsigned int));
	Common::PrintGLError();

	indiceCount = accumulateIndicesSize;
	
	return true;
}

bool MyModel::InitInstanced(std::string fileName, int num_of_maxInstance, std::string matFileName)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	const char * matFilePtr = NULL;
	if (matFileName != "")
		matFilePtr = matFileName.c_str();

	bool ret = tinyobj::LoadObj(shapes, materials, err, fileName.c_str(), matFilePtr);
	if (!ret)
	{
		printf("Load Models Fail! Please check the solution path");
		return false;
	}

	printf("Load Models Success ! Shapes size %d Maerial size %d\n", shapes.size(), materials.size());

	unsigned int accumulateDataSize = 0, accumulateIndicesSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		for (unsigned int &index : shapes[i].mesh.indices)
		{
			index += accumulateDataSize / 3;
		}
		accumulateDataSize += shapes[i].mesh.positions.size();
		accumulateIndicesSize += shapes[i].mesh.indices.size();
	}


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, accumulateDataSize * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	unsigned int previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.positions.size() * sizeof(float), &(shapes[i].mesh.positions[0]));
		previousDataSize += shapes[i].mesh.positions.size() * sizeof(float);
	}
	assert(previousDataSize == accumulateDataSize * sizeof(float));
	Common::PrintGLError();
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, accumulateDataSize * sizeof(float), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.normals.size() * sizeof(float), &(shapes[i].mesh.normals[0]));
		previousDataSize += shapes[i].mesh.normals.size() * sizeof(float);
	}
	assert(previousDataSize == accumulateDataSize * sizeof(float));
	Common::PrintGLError();
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vboModel);
	glBindBuffer(GL_ARRAY_BUFFER, vboModel);
	glBufferData(GL_ARRAY_BUFFER, num_of_maxInstance * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	Common::PrintGLError();
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, accumulateIndicesSize * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
	previousDataSize = 0;
	for (int i = 0; i < shapes.size(); i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, previousDataSize, shapes[i].mesh.indices.size() * sizeof(unsigned int), &(shapes[i].mesh.indices[0]));
		previousDataSize += shapes[i].mesh.indices.size() * sizeof(unsigned int);
	}
	assert(previousDataSize == accumulateIndicesSize * sizeof(unsigned int));
	Common::PrintGLError();

	indiceCount = accumulateIndicesSize;
	ModelMatrix = new glm::mat4[num_of_maxInstance];
	FinalModelMatrix = new glm::mat4[num_of_maxInstance];
	maxInstance = num_of_maxInstance;

	return true;
}

void MyModel::Render()
{
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0, currentCount);
	glBindVertexArray(0);
}

glm::mat4 MyModel::getModelMatrix()
{
	if (dirty)
	{
		cache = parentModel * currentModel;
	}
	return cache;
}


void MyModel::setInstanceModel(glm::mat4 &model, int index)
{
	if (index < maxInstance)
		ModelMatrix[index] = model;
	else
		cout << "Index is larger than MaxInstance: " << maxInstance << endl;
}

void MyModel::bufferInstanceModel()
{
	for (int i = 0; i < maxInstance; i++)
	{
		FinalModelMatrix[i] = parentModel * ModelMatrix[i];
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vboModel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, maxInstance * sizeof(glm::mat4), FinalModelMatrix);

	glBindVertexArray(0);
}

void MyModel::RenderInstance(int count)
{
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0, count);
	glBindVertexArray(0);
}