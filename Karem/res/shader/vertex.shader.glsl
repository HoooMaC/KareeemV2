#version 450 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int aTexIndex;
layout (location = 4) in int aEntityId;

uniform mat4 uProjectionView;

out vec4 vColor;
out vec2 vTexCoord;
flat out int vTexIndex;
flat out int vEntityId;

void main()
{
    gl_Position = uProjectionView  * aPos;
    vColor = aColor;    
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vEntityId = aEntityId;
}
