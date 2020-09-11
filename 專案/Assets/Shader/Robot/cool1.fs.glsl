/*
 * Original shader from: https://www.shadertoy.com/view/MdKyzw
 */

#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution resolution

// 物體參數
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

// 光參數
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
} light;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
} vertexIn;
uniform vec3 cameraPos;

// --------[ Original ShaderToy begins here ]---------- //
vec2 rand2(in vec2 p)
{
	return fract(vec2(sin(p.x * 591.32 + p.y * 154.077), cos(p.x * 391.32 + p.y * 49.077)));
}

float voronoi(in vec2 x)
{
	vec2 p = floor(x);
	vec2 f = fract(x);
	float minDistance = 1.;

	for (int j = -1; j <= 1; j++)
	{
		for (int i = -1; i <= 1; i++)
		{
			vec2 b = vec2(i, j);
			vec2 rand = .5 + .5 * sin(iTime * 3. + 12. * rand2(p + b));
			vec2 r = vec2(b) - f + rand;
			minDistance = min(minDistance, length(r));
		}
	}
	return minDistance;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	//////////// Get Main Color ////////////
	vec2 uv = fragCoord / iResolution.xy;
	uv.x *= iResolution.x / iResolution.y;
	float val = pow(voronoi(uv * 8.) * 1.25, 7.) * 2.;
	float gridLineThickness = 2. / iResolution.y;
	vec2 grid = step(mod(uv, .05), vec2(gridLineThickness));
	//vec2 grid = step(mod(uv, .05), vec2(gridLineThickness));

	fragColor = vec4(vec3(0.8) - vec3(0, val * (grid.x + grid.y), 0), 1.);
	//fragColor = vec4(vec3(0.8) - vec3(0, 1.0 * (grid.x + grid.y), 0), 1.);

	//////////// Lighting ////////////
	light.position = vec3(5.f, 0, 0);
	light.ambient = vec3(1);
	light.diffuse = vec3(0.5f);
	light.specular = vec3(1.f);

	material.diffuse = vec3(1.0);
	material.specular = vec3(0.9);
	material.ambient = vec3(1.0);
	material.shininess = 64.f;

	// ambient
	vec3 ambient = light.ambient * fragColor.rgb;

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

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result * fragColor.rgb, 1.f);
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
	mainImage(gl_FragColor, gl_FragCoord.xy);
}