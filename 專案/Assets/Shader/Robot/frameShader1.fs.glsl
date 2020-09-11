#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int mode;
uniform float time;


void main()
{    
    // 負片
    if (mode == 1)
    {
        FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
    }
    // 灰階
    if (mode == 2)
    {
        vec3 color = vec3(texture(screenTexture, TexCoords));
        FragColor = vec4(vec3(color.x * 0.299 + color.y * 0.587 + color.z * 0.114), 1.0);
    }
    // 高斯模糊
    else if (mode == 3)
    {
        float gaussianMat[25] = float[] (
            1,4,7,4,1,
            4,16,26,16,4,
            7,26,41,26,7,
            4,16,26,16,4,
            1,4,7,4,1
        );
        vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
        vec3 result = vec3(0); // current fragment's contribution

        for(int i = -2; i <= 2; ++i)
        {
            for(int j = -2; j <= 2; j++)
            {
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * gaussianMat[(i+2)*5+(j+2)];
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * gaussianMat[(i+2)*5+(j+2)];
            }
        } 
        result = result / 273.f;
        FragColor = vec4(result, 1.0);
    }
    // Sharpen
    else if (mode == 4)
    {
        float sharpenKernel[9] = float[] (
           -1,-1,-1,
           -1, 9,-1,
           -1,-1,-1
        );
        vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
        vec3 result = vec3(0); // current fragment's contribution
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <=1; j++)
            {
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * sharpenKernel[(i+1)*3+(j+1)];
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * sharpenKernel[(i+1)*3+(j+1)];
            }
        }
        FragColor = vec4(result, 1.0);
    }
    // Edge Detection
    else if (mode == 5)
    {
        float edgeKernel[9] = float[] (
           1,1,1,
           1,-8,1,
           1,1,1
        );
        vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
        vec3 result = vec3(0); // current fragment's contribution
        for(int i = -1; i <= 1; i++)
        {
            for(int j = -1; j <=1; j++)
            {
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * edgeKernel[(i+1)*3+(j+1)];
                result += texture(screenTexture, TexCoords + vec2(tex_offset.x * j, tex_offset.y * i)).rgb * edgeKernel[(i+1)*3+(j+1)];
            }
        }
        FragColor = vec4(result, 1.0);
    }
    // Cycle
    else if (mode == 6)
    {
        vec2 tex_offset = 1.0 / textureSize(screenTexture, 0); // gets size of single texel
        float thickness = 40.f;
        float tX = 2.f * (TexCoords.x - 0.5), tY = 2.f * (TexCoords.y - 0.5);
        float dist = pow(tX * tX + tY * tY, 0.5) * 400;

        float radians = ((time+dist) - int(time+dist)/int(thickness)*int(thickness) )/thickness * 3.1415;
        float sampleOffset = cos(radians);
        if (radians > 1.57)
            FragColor = texture(screenTexture, TexCoords.xy + vec2(sampleOffset * (thickness/2) * tex_offset));
        else
            FragColor = texture(screenTexture, TexCoords.xy - vec2(sampleOffset * (thickness/2) * tex_offset));
    }
}