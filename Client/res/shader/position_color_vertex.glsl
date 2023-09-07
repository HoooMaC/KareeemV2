#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uProjectionView;
uniform mat4 uModel;

out vec2 vTexCoord;

void main()
{
    gl_Position = uProjectionView  * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
}
