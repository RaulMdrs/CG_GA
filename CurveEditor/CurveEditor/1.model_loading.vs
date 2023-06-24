#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 ortho;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
    