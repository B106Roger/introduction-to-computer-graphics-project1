#pragma once
#include <Common.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	 bool Init();
	 bool AddShader(GLenum shaderType, std::string fileName);
	 bool Finalize();
	 void Enable();
	 void Disable();

	 void setUniformVec2(string target, const glm::vec2 &value);
	 void setUniformVec3(string target, const glm::vec3 &value);
	 void setUniformVec4(string target, const glm::vec4 &value);
	 void setUniformMat3(string target, const glm::mat3 &value);
	 void setUniformMat4(string target, const glm::mat4 &value);
	 void setUniformInt      (string  target, int value);
	 void setUniformFloat  (string  target, float value);




private:
	std::vector<GLuint> shaderList;
	GLuint program;
};

