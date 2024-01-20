#version 450 core

in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;

layout (location = 0) out vec4 oFragColor;
layout (location = 1) out vec4 oColor2;
layout (location = 2) out vec4 oColor3;
layout (location = 3) out int oEntityId;

uniform sampler2D uTexture[32];

void main()
{
    oEntityId = 1000;
    int texIndex = int(vTexIndex);
    oFragColor = texture(uTexture[texIndex], vTexCoord) * vColor;
    oColor2 = vec4(0.8, 0.2, 0.3, 1.0);
    oColor3 = vec4(0.2, 0.1, 0.8, 1.0);
}
