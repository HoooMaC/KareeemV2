#version 450 core

layout (location = 0) in vec3 aPos;

uniform mat4 uProjectionView;
uniform mat4 uModel;
void main()
{
    gl_Position = uProjectionView * uModel * vec4(aPos, 1.0);
    //gl_Position = uProjectionView * vec4(aPos, 1.0);
}
