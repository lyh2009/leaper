#version 330 core

layout(location = 0) out vec4 FragColor;
out int EntityID;

in vec4 v_Color;
in flat int v_EntityID;

void main()
{
    FragColor = v_Color;
    EntityID = 0;
}