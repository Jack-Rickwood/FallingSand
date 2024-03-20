#version 140

uniform sampler2D original;
uniform sampler2D blurred;
uniform float exposure = 0.75;
uniform float saturation = 2.0f;

void main()
{
    vec2 TexCoords = gl_TexCoord[0].xy;
    const float gamma = 2.2;

    vec3 original_color = texture2D(original, TexCoords).rgb;      
    vec3 blurred_color = texture2D(blurred, TexCoords).rgb;

    original_color += blurred_color;

    vec3 result = vec3(1.0) - exp(-original_color * exposure);
  
    result = pow(result, vec3(1.0 / gamma));

    float lum = result.r * 0.2 + result.g * 0.7 + result.b * 0.1;
	vec3 diff = result.rgb - vec3(lum);
	gl_FragColor = vec4(vec3(diff) * saturation + lum, 1.0);
}  