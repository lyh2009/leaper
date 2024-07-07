#version 450 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color   ;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in int   a_EntityID;
layout(location = 5) in vec3  a_AnimInfo;

out vec4 		v_Color;
out vec2 		v_TexCoord;
out flat float 	v_TexIndex;
out flat int 	v_EntityID;

uniform mat4 u_ViewProjection;

void main()
{
	int currentCol = int(a_AnimInfo.z) % int(a_AnimInfo.x);
	int currentRow = int(a_AnimInfo.z) / int(a_AnimInfo.y);

	float us = 1.0 / a_AnimInfo.x; 
	float vs = 1.0 / a_AnimInfo.y;

	v_TexCoord = a_TexCoord * vec2(us,vs);
	v_TexCoord.x += float(currentCol) * us; 
	v_TexCoord.y += float(currentRow) * vs; 

    v_TexCoord = a_TexCoord;
	v_Color    = a_Color;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}