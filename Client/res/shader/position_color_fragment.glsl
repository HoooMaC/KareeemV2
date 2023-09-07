#version 450 core

uniform sampler2D uTexture;

in vec2 vTexCoord;

out vec4 FragColor;


void main()
{
	FragColor = texture(uTexture, vTexCoord);
}