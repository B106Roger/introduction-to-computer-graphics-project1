#pragma once
#include <vector>
#include <ShaderObject.h>
#include <string>
using namespace std;

class DrawModelShader: public ShaderObject
{
public:
	DrawModelShader();
	~DrawModelShader();

	bool Init();
	bool LoadTexture(string);
	bool InitUniformLocation();
	void SetMMat(const glm::mat4& mat);
	void SetPMat(const glm::mat4& mat);
	void SetCameraPos(const glm::vec3& vec);
	void SetViewMat4(glm::mat4 &val) { viewMat4 = val; }
	glm::mat4 GetViewMat4() { return viewMat4; }

	void setUniformVec2(string target, const glm::vec2 &value);
	void setUniformVec3(string target, const glm::vec3 &value);
	void setUniformVec4(string target, const glm::vec4 &value);
	void setUniformMat3(string target, const glm::mat3 &value);
	void setUniformMat4(string target, const glm::mat4 &value);
	void setUniformInt(string  target, int value);
	void setUniformFloat(string  target, float value);
	std::vector<GLuint> textures;
private:
	GLuint um4pLocation;
	GLuint um4mvLocation;
	GLuint um4mLocation;
	GLuint um3nLocation;
	GLuint cameraPosLocation;

	glm::mat4 viewMat4;
};

