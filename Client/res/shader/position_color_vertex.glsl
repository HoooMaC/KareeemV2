#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

uniform mat4 uProjectionView;
uniform mat4 uModel;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;

void main()
{
    vTexCoord = aTexCoord;
    vColor = aColor;
    vTexIndex = aTexIndex;
    gl_Position = uProjectionView  * vec4(aPos, 1.0);
}
