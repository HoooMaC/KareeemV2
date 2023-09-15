#version 450 core

in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;

out vec4 FragColor;

uniform sampler2D uTexture[32];

void main()
{
    int texIndex = int(vTexIndex);
    if (texIndex == 0)
    {
        FragColor = vColor;
    }
    else
    {
        FragColor = texture(uTexture[texIndex], vTexCoord);
        //FragColor = vec4(vTexCoord, 0.0f, 1.0f);
    }
}
