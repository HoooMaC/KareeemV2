#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * mat4(1.0f) * vec4(aPos, 1.0);
}
