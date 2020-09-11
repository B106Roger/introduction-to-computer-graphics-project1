#version 410 core


#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution resolution

// --------[ Original ShaderToy begins here ]---------- //
// Fork of AA Grid shadertoy.com/view/Md2XWt

// The complex plane, as a black & white grid, under the action of the holomorphic map f : z -> z^2

// Notice how all squares intersect at right angles, even on the deformed parts!
// A map that preserves angles like that is called a conformal map
// I think that all conformal functions are holomorphic,
// and that the converse holds everywhere the Jacobian is invertible

#define HOLOMORPHIC_MAP  // Undefine to see a vanilla grid!
//#define LINES  // Define to see lines, not a checkerboard!

#define SCALE 4.
// 砰把计
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

// 把计
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

void mainImage(out vec4 fragColor, in vec2 fragCoord){
	//////////// Get Main Color ////////////
    vec2 uv = SCALE *(2. * fragCoord - iResolution.xy) / iResolution.y;  // The Fabrice map!

    #ifdef HOLOMORPHIC_MAP
    uv = vec2(uv.x * uv.x - uv.y * uv.y, 2. * uv.x * uv.y);  // Holomorphic map f : z -> z^2
    #endif

    uv.x -= .4 * iTime;  // Move the til2pes along the u-direction, just for teh lulz

    // I don't understand this part at all. Someone care to explain, please?
    vec2 sine_uv = sin(uv);
    vec2 jacobian = fwidth(sine_uv);

    #ifdef LINES
    sine_uv = abs(sine_uv) - jacobian;
    #endif

    vec2 uv_aa = smoothstep(-jacobian, jacobian, sine_uv);  // Original version
    //vec2 uv_aa = smoothstep(-1., 1., sine_uv / jacobian);  // Fabrice's version
    //vec2 uv_aa = sine_uv / jacobian;  // No antialiasing version

    uv_aa = 2. * uv_aa - 1.;  // Remap to [-1;1]
    float checkerboard = .1 * uv_aa.x * uv_aa.y + .7;  // Combine sine waves and remap to [0; 1]
    fragColor.rgb = vec3(checkerboard)*abs(sin(uv.y+time));

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
    gl_FragColor.a = 1.;
}