#version 410 core

out vec4 fragColor;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
	vec3 vUv;
} vertexOut;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

struct Material
{
	vec3 specular;
	vec3 ambient;
	vec3 diffuse;
	float shininess;
} material;

uniform vec3 cameraPos;

void main()
{
	light.position = vec3(0, 0, 10.0f);
	light.ambient = vec3(0.1f);
	light.diffuse = vec3(0.8f);
	light.specular = vec3(1.0f, 1.0f, 1.0f);

	material.diffuse = vec3(1.0);
	material.specular = vec3(0.5f, 0.5f, 0.5f);
	material.ambient = vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 20.f;

	vec3 ambient = vec3(0.5);

	vec3 norm = vertexOut.vNormal;
	vec3 lightDir = normalize(vec3(0) - vertexOut.vPos);//
	float diff = max(dot(norm, lightDir), 0.0);//
	vec3 diffuse = light.diffuse * (diff * material.diffuse);//

	// specular
	vec3 viewDir = normalize(cameraPos - vertexOut.vPos);//
	vec3 reflectDir = reflect(-lightDir, norm);//
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//
	vec3 specular = light.specular * (spec * material.specular);//

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.f);
}