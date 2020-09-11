#version 430 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

uniform mat3 um3n;
uniform mat4 um4mv;
uniform mat4 um4m;
uniform mat4 um4p;
uniform vec3 cameraPos;


out VertexData
{
	vec3 vNormal;
	vec3 vPos;
} vertexOut;


void main()
{
	vec4 posV4 = vec4(vertex, 1.0);

	vertexOut.vNormal = normalize(um3n * normal);
	vertexOut.vPos = vec3(um4m * posV4);

	gl_Position = um4p * um4mv * posV4 + um4m * vec4(um3n * cameraPos * 0.00001, 0) * 0.00001;
	// vec4 = um3n * um4m * cameraPos;
}
