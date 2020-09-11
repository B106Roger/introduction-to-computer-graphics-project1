#version 410 core

// 聲明input primitive類型
layout(triangles) in;
// 聲明output primitive類型，output頂點數量多於max_vertices會被忽略
layout(triangle_strip, max_vertices=9) out;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
    vec4 lightSpacePos;
}  vertexOut[];

out VertexDataN
{
	vec3 vNormal;
	vec3 vPos;
	vec3 gNormal;
    vec4 lightSpacePos;
} vertexOutO;

void main()
{
    vec3 v1 = vertexOut[0].vPos - vertexOut[1].vPos;
    vec3 v2 = vertexOut[0].vPos - vertexOut[2].vPos;
    vec3 gNormal = normalize(cross(v2, v1));
	for(int i =0; i <3; i++)
    {
        gl_Position = gl_in[i].gl_Position;
        vertexOutO.vNormal = vertexOut[i].vNormal;
        vertexOutO.vPos = vertexOut[i].vPos;
        vertexOutO.lightSpacePos = vertexOut[i].lightSpacePos;
        vertexOutO.gNormal = gNormal;
        EmitVertex();
    }
    EndPrimitive( );
}
