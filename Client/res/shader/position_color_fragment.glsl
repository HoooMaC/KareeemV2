#version 450 core

in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;

out vec4 FragColor;

void main()
{
	if(vTexIndex == 0.0f)
	{
		FragColor = vColor;
	}
	else
	{
		FragColor = vec4(vTexCoord, vTexCoord);
		FragColor = vec4(1.0f);
	}
}