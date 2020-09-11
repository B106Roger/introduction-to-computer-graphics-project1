#version 410 core

layout(location = 0) in vec3 vertex;

uniform mat4 um4mv;
uniform mat4 um4p;

out VertexData
{
	vec3 texcoord;
} vertexData;

void main()
{
	mat4 myMat = um4mv;
	//myMat[3][3] = 0.f;
	vec4 position = um4p * myMat * vec4(vertex, 1.0);
	gl_Position = position.xyww;

	vertexData.texcoord = vec3(vertex.x, -vertex.y, -vertex.z);
}
