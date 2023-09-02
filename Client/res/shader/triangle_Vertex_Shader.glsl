#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 uProjectionView;
uniform mat4 uModel;

out vec4 vColor;

void main()
{
    vColor = aColor;
    gl_Position = uProjectionView * uModel * vec4(aPos, 1.0);
}
