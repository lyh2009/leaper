#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 lightSpaceMat;
uniform mat4 u_Trans;

void main()
{
    gl_Position = lightSpaceMat * u_Trans * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

void main() {}