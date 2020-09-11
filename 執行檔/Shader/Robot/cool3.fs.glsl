//// lez
///*
// * Original shader from: https://www.shadertoy.com/view/XdSfWW
// */
//
//#extension GL_OES_standard_derivatives : enable
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
} light;

in VertexData
{
	vec3 vNormal;
	vec3 vPos;
} vertexIn;
uniform vec3 cameraPos;

void mainImage(out vec4 fragColor, in vec2 fragCoord){
	//////////// Get Main Color ////////////
    vec2 position = (gl_FragCoord.xy/resolution.xy) -0.5;
	
	float y = 0.2*position.y * sin(25.0*position.y - 20.0*(time * 3.0)*0.1);
	
	y = 1.0 / (100. * abs(position.x - y));
	y += 0.1/(065.*length(position - vec2(0., position.y)));
	
	float saule = 1./(65.*length(position - vec2(0, 0)));
	vec4 vsaule = vec4(saule*5., saule, saule, 1.0);
	vec4 vstari = vec4(y*5.,position.y*0.5 - y, y, 1.0);
    fragColor = mix(vsaule, vstari, 1.0);

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