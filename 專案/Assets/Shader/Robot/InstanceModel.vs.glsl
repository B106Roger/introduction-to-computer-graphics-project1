#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in mat4 model;

uniform mat4 um4v;
uniform mat4 um4p;
uniform mat4 lightSpaceMatrix;

out VertexData
{
	vec3 vNormal;
	vec3 vPos;
	vec4 lightSpacePos;
} vertexOut;

void main()
{
	vec4 posV4 = vec4(vertex, 1.0);

	vertexOut.vNormal = normalize(transpose(inverse(mat3(model))) * normal);
	vertexOut.vPos = vec3(model * posV4);
	vertexOut.lightSpacePos = lightSpaceMatrix * posV4;

	gl_Position = (um4p * (um4v * (model * posV4)));
}
