#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewContentBroswerion;
};

out vec3 v_TexCoord;

uniform mat4 u_View;
uniform mat4 u_ContentBroswerion;

void main()
{

    v_TexCoord  = a_Position;
    vec4 pos    = u_ContentBroswerion * u_View * vec4(a_Position, 1.0);
    gl_Position = pos;
}

#type fragment
#version 450 core
out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, v_TexCoord);
}