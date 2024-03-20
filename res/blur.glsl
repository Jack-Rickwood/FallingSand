#version 140

uniform sampler2D image;
  
uniform bool horizontal;
uniform float weight[9] = float[] (0.224903, 0.191866, 0.119119, 0.0538137, 0.0176862, 0.00422746, 0.000734649, 0.0000927817, 0.0000085122);
uniform float blur_coeff = 0.75;

void main()
{
    vec2 TexCoords = vec2(gl_TexCoord[0]).xy;
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture2D(image, TexCoords).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 9; ++i)
        {
            result += texture2D(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * blur_coeff;
            result += texture2D(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i] * blur_coeff;
        }
    }
    else
    {
        for(int i = 1; i < 9; ++i)
        {
            result += texture2D(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i] * blur_coeff;
            result += texture2D(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i] * blur_coeff;
        }
    }
    gl_FragColor = vec4(result, 1.0);
}