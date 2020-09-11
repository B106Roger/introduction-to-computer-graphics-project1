#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in mat4 model;

uniform mat4 lightSpaceMatrix;

void main()
{
	vec4 posV4 = vec4(vertex, 1.0);

	gl_Position = (lightSpaceMatrix * (model * posV4));
}
