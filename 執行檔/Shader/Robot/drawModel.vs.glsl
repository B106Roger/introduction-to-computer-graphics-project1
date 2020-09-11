#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 position;

uniform mat3 um3n;
uniform mat4 um4v;
uniform mat4 um4m;
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
	vec4 transferResult = um4m * vec4(vertex, 1.0) + vec4(position, 0.0);

	vertexOut.vNormal = normalize(um3n * normal);
	vertexOut.vPos = transferResult.xyz;
	vertexOut.lightSpacePos = lightSpaceMatrix * vec4(vertexOut.vPos, 1.0);
	gl_Position = um4p * um4v * transferResult;
}
