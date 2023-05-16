#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;

void main()
{

    vec3 result = texture(texture_diffuse, TexCoords).rgb;

    FragColor = vec4(result, 1.0);
}
