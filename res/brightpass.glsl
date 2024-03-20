#version 140

uniform sampler2D texture;
uniform float threshold = 0.25f;

void main()
{
    vec3 color = texture2D(texture, gl_TexCoord[0].xy).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > threshold)
        gl_FragColor = vec4(color, 1.0);
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}