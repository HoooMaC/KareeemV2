#version 450 core

in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;
in flat int vEntityId;

layout (location = 0) out vec4 oFragColor;
layout (location = 1) out int oEntityId;

uniform sampler2D uTexture[32];

void main()
{
    int texIndex = int(vTexIndex);
    oFragColor = texture(uTexture[texIndex], vTexCoord) * vColor;
    oEntityId = vEntityId;
}
