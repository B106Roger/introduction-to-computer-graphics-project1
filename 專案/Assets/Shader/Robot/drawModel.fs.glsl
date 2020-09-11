#version 410 core

out vec4 fragColor;

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution resolution
#define PI 3.141592654

in VertexDataN
{
	vec3 vNormal;
	vec3 vPos;
	vec3 gNormal;
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
} ;

uniform samplerCube texs;
uniform sampler2D shadowMap;

uniform vec3 cameraPos;
uniform float eta;
uniform bool reflectOrRefract;
uniform int mode;
uniform Light light;
uniform bool reverseNormal;

// mainImage3
float ShadowCalculation(vec4 fragPosLightSpace);
vec2 rand2(in vec2 p);
float voronoi(in vec2 x);
// mainImage4
vec2 Rot(vec2 v, float angle);
vec3 DrawStar(float len, float angle);
float map(float l);
vec3 DrawCloud(float dis, float angle, vec2 coord);
vec3 Render(vec2 coord);
// mainImage6
float hash( float n );
float noise( in vec2 x );
float fbm(vec2 p, vec3 a);
vec3 drawLines( vec2 uv, vec3 fbmOffset, vec3 color1, vec3 color2 );

// green grid
vec3 mainImage1(in vec2 fragCoord);
// black white block
vec3 mainImage2(in vec2 fragCoord);
// vertical purple lightening
vec3 mainImage3(in vec2 fragCoord);
// vertical tower
vec3 mainImage4(in vec2 fragCoord);
// multi cycle around middle
vec3 mainImage5(in vec2 fragCoord);
// horizontal lightening
vec3 mainImage6(in vec2 fragCoord);



void main(void) 
{
	material.specular = vec3(0.5f, 0.5f, 0.5f);
	material.ambient = vec3(0.5f, 0.5f, 0.5f);
	material.shininess = 20.f;

	// ambient
	vec3 ambient;
	if (mode == 0 || mode == 4 || mode == 3 || mode == 5)
	{
		ambient = light.ambient * material.ambient;
		material.diffuse = vec3(1.0f, 1.0f, 1.0f);
	}
	else if (mode == 1 || mode == 2)
	{
		vec3 R;
		material.shininess = 40.f;
		if (mode == 1)
		{
			R = reflect(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal));
		}
		else if (mode == 2)
		{
			R = refract(normalize(vertexIn.vPos - cameraPos), normalize(vertexIn.vNormal), eta);
		}
		R *= vec3(1, -1, -1);
		ambient = light.ambient * texture(texs, R).rgb;
		material.diffuse = texture(texs, R).rgb;
	}

	// diffuse 
	vec3 norm = normalize(vertexIn.vNormal);
	vec3 lightDir = normalize(light.position - vertexIn.vPos);//
	float diff = max(dot(norm, lightDir), 0.0);//
	vec3 diffuse = light.diffuse * (diff * material.diffuse);//
	if (diff == 0.f)
	{
		material.diffuse = vec3(0.1, 0.1, 0.1);
		diff = max(dot(-norm, lightDir), 0.0);//
		diffuse = light.diffuse * (diff * material.diffuse);//
	}

	// specular
	vec3 viewDir = normalize(cameraPos - vertexIn.vPos);//
	vec3 reflectDir = reflect(-lightDir, norm);//
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//
	vec3 specular = light.specular * (spec * material.specular);//

	// shadow
	float shadowFactor = ShadowCalculation(vertexIn.lightSpacePos);
	vec3 result = ambient + (diffuse + specular) * (1.0 - shadowFactor);
	fragColor = vec4(result, 1.f);

	if (mode == 3)
	{
		fragColor = vec4(mix(result, mainImage6(gl_FragCoord.xy), 0.8) , 1.0);
	}
	else if (mode == 4)
	{
		fragColor = vec4(mix(result, mainImage4(gl_FragCoord.xy), 0.7) , 1.0);
	}
	else if (mode == 5)
	{
		fragColor = vec4(mix(result, mainImage3(gl_FragCoord.xy), 0.7) , 1.0);
	}
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
	float bias = 0.00001;
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


// mainImage 6
float hash( float n ) { return fract(sin(n)*753.5453123); }

// Slight modification of iq's noise function.
float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*157.0;
    return mix(
                    mix( hash(n+  0.0), hash(n+  1.0),f.x),
                    mix( hash(n+157.0), hash(n+158.0),f.x),
            f.y);
}


float fbm(vec2 p, vec3 a)
{
     float v = 0.0;
     v += noise(p*a.x)*1.00;
     v += noise(p*a.y)*1.00;
     v += noise(p*a.z)*.125;
     return v;
}

vec3 drawLines( vec2 uv, vec3 fbmOffset, vec3 color1, vec3 color2 )
{
    float timeVal = time * 0.2;
    vec3 finalColor = vec3( 0.0 );
    for( int i=0; i < 3; ++i )
    {
        float indexAsFloat = float(i);
        float amp = 40.0 + (indexAsFloat*10.0);
        float period = 2.0 + (indexAsFloat+2.0);
        float thickness = mix( 0.9, 1.0, noise(uv*10.0) );
        float t = abs( 0.9 / (sin(uv.x + fbm( uv + timeVal * period, fbmOffset )) * amp) * thickness );
        
        finalColor +=  t * color1;
    }
    
    for( int i=0; i < 0; ++i )
    {
        float indexAsFloat = float(i);
        float amp = 40.0 + (indexAsFloat*7.0);
        float period = 2.0 + (indexAsFloat+8.0);
        float thickness = mix( 0.7, 1.0, noise(uv*10.0) );
        float t = abs( 0.8 / (sin(uv.x + fbm( uv + timeVal * period, fbmOffset )) * amp) * thickness );
        
        finalColor +=  t * color2 * 0.6;
    }
    
    return finalColor;
}


vec3 mainImage1(in vec2 fragCoord)
{
	//////////// Get Main Color ////////////
	vec2 uv = fragCoord / iResolution.xy;
	uv.x *= iResolution.x / iResolution.y;
	float val = pow(voronoi(uv * 8.) * 1.25, 7.) * 2.;
	float gridLineThickness = 2. / iResolution.y;
	vec2 grid = step(mod(uv, .05), vec2(gridLineThickness));
	//vec2 grid = step(mod(uv, .05), vec2(gridLineThickness));

	return vec3(0.8) - vec3(0, val * (grid.x + grid.y), 0);
}

vec3 mainImage2(in vec2 fragCoord) {
	float SCALE = 4.f;
	//////////// Get Main Color ////////////
	vec2 uv = SCALE * (2. * fragCoord - iResolution.xy) / iResolution.y;  // The Fabrice map!

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
	return vec3(checkerboard)*abs(sin(uv.y + time));
}

vec3 mainImage3(in vec2 fragCoord)
{
	float SCALE = 4.f;
	//////////// Get Main Color ////////////
	vec2 position = (gl_FragCoord.xy / resolution.xy) - 0.5;

	float y = 0.2*position.y * sin(25.0*position.y - 20.0*(time * 3.0)*0.1);

	y = 1.0 / (100. * abs(position.x - y));
	y += 0.1 / (065.*length(position - vec2(0., position.y)));

	float saule = 1. / (65.*length(position - vec2(0, 0)));
	vec4 vsaule = vec4(saule*5., saule, saule, 1.0);
	vec4 vstari = vec4(y*5., position.y*0.5 - y, y, 1.0);
	return mix(vsaule, vstari, 1.0).bgr;
}

vec3 mainImage4(in vec2 fragCoord)
{
    vec2 position = (fragCoord.xy/resolution.xy) -0.5;
	
	float y = 0.2*position.y * sin(300.0*position.y - 20.0*time*0.1);
	
	y = 1.0 / (600. * abs(position.x - y));
	y += 1./(665.*length(position - vec2(0., position.y)));
	
	float saule = 1./(65.*length(position - vec2(0, 0)));
	vec4 vsaule = vec4(saule, saule, saule*5., 1.0);
	vec4 vstari = vec4(position.y*0.5 - y, y, y*5., 1.0);
	return mix(vsaule, vstari, 0.7).rgb;
}

vec3 mainImage5(in vec2 fragCoord)
{
	float t = time;
	vec2 r = resolution;
	
    vec2 p = (fragCoord * 2.0 - r) / min(r.x, r.y);
	vec3 destColor = vec3(0.5, 0.5, 0.7);
	float f = 0.0;
    	for(float i = 0.0; i < 20.0; i++){
        float s = sin(t + i * 0.314) * 0.5;
        float c = cos(t + i * 0.314) * 0.5;
        f += 0.002 / abs(length(p + vec2(s, c)) - 1.5*abs(sin(0.3 * t)));
    	}
    	return  vec3(destColor * f);
}

vec3 mainImage6(in vec2 fragCoord)
{
	float mytime = time * 1000;
	vec2 uv = ( fragCoord / resolution.xy ) * 1.0 - 1.5;
    uv.x *= resolution.x/resolution.y;
    uv.xy = uv.yx;

    vec3 lineColor1 = vec3( 2.3, 0.5, .5 );
    vec3 lineColor2 = vec3( 0.3, 0.5, 2.5 );
    
    vec3 finalColor = vec3(0.0);

    
    float t = sin( mytime ) * 0.5 + 0.5;
    float pulse = mix( 0.10, 0.20, t);
    
    //finalColor += drawLines( uv, vec3( 1.0, 20.0, 30.0), lineColor1, lineColor2 ) * pulse;
    finalColor += drawLines( uv, vec3( 1.0, 1.0, 8.0), lineColor2, lineColor2 );
    
    return  finalColor;
}