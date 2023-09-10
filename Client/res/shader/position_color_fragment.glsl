#version 450 core

uniform sampler2D uTexture;

in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;

out vec4 FragColor;

void main()
{
	if(vTexIndex == 0)
	{
		FragColor = vColor;
	}
	else
	{
		FragColor = texture(uTexture, vTexCoord);
	}
}