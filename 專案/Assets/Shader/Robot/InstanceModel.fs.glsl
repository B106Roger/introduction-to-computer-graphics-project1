#version 410 core

out vec4 fragColor;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
	vec4 lightSpacePos;
} vertexIn;

// ??砰把计
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

// ??把计
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



uniform samplerCube tex;
uniform sampler2D shadowMap;
uniform vec3 cameraPos;
uniform float eta;
uniform bool reflectOrRefract;
uniform Light light;

float ShadowCalculation(vec4 fragPosLightSpace);

void main(void) 
{
	// light.position = vec3(0, 0, 10.0f);
	// light.ambient = vec3(0.1f);
	// light.diffuse = vec3(0.8f);
	// light.specular = vec3(1.0f, 1.0f, 1.0f);

	material.diffuse = vec3(1.0);
	material.specular = vec3(0.5f, 0.5f, 0.5f);
	material.ambient = vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 20.f;

	vec3 R;
	if (reflectOrRefract)
	{
		R = reflect(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal));
	}
	else
	{
		R = refract(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal), eta);
	}
	R *= vec3(1, -1, -1);
	// fragColor = texture(tex, R);

	// ambient
	vec3 ambient = light.ambient * texture(tex, R).rgb;

	// diffuse 
	vec3 norm = normalize(vertexIn.vNormal);
	vec3 lightDir = normalize(light.position - vertexIn.vPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	vec3 viewDir = normalize(cameraPos - vertexIn.vPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	float shadow = ShadowCalculation(vertexIn.lightSpacePos);
	vec3 result = ambient + (diffuse + specular) * (1 - shadow);
	fragColor = vec4(result, 1.f);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片段是否在阴影中
	float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth  ? 0.7 : 0.0;

    // PCF
    // float shadow = 0.0;
    // vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
    //         shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
    //     }    
    // }
    // shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}