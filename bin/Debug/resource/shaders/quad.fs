#version 330 core

layout (location = 0) out vec4 FragColor;
out int EntityID;

in vec3 		v_Position;
in vec4 		v_Color;
in vec2 		v_TexCoord;
in flat float 	v_TexIndex;
in flat int 	v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

struct PointLight
{
	vec3 position;
	vec3 color;
	float intensity;
};
const int MAX_POINT_LIGHT_COUNT = 4;

uniform PointLight u_PointLight[MAX_POINT_LIGHT_COUNT];
uniform vec3 u_AmbientLight;

vec3 result;

vec3 CalcPointLight(PointLight point_light, vec4 tex_color)
{
	vec3 light_pos = point_light.position;
	vec3 light_color = point_light.color;
	float intensity = point_light.intensity;

	float distance = distance(light_pos.xy, v_Position.xy);
    float diffuse = 0.0;
    if (distance <= intensity)
    	diffuse =  1.0 - abs(distance / intensity);
	
   	return min(tex_color.rgb * (light_color * diffuse + u_AmbientLight), tex_color.rgb);
}

void main()
{
    vec4 texColor = v_Color;

	switch(int(v_TexIndex))
	{
		case  0: texColor *= texture2D(u_Textures[ 0], v_TexCoord); break;
		case  1: texColor *= texture2D(u_Textures[ 1], v_TexCoord); break;
		case  2: texColor *= texture2D(u_Textures[ 2], v_TexCoord); break;
		case  3: texColor *= texture2D(u_Textures[ 3], v_TexCoord); break;
		case  4: texColor *= texture2D(u_Textures[ 4], v_TexCoord); break;
		case  5: texColor *= texture2D(u_Textures[ 5], v_TexCoord); break;
		case  6: texColor *= texture2D(u_Textures[ 6], v_TexCoord); break;
		case  7: texColor *= texture2D(u_Textures[ 7], v_TexCoord); break;
		case  8: texColor *= texture2D(u_Textures[ 8], v_TexCoord); break;
		case  9: texColor *= texture2D(u_Textures[ 9], v_TexCoord); break;
		case 10: texColor *= texture2D(u_Textures[10], v_TexCoord); break;
		case 11: texColor *= texture2D(u_Textures[11], v_TexCoord); break;
		case 12: texColor *= texture2D(u_Textures[12], v_TexCoord); break;
		case 13: texColor *= texture2D(u_Textures[13], v_TexCoord); break;
		case 14: texColor *= texture2D(u_Textures[14], v_TexCoord); break;
		case 15: texColor *= texture2D(u_Textures[15], v_TexCoord); break;
		case 16: texColor *= texture2D(u_Textures[16], v_TexCoord); break;
		case 17: texColor *= texture2D(u_Textures[17], v_TexCoord); break;
		case 18: texColor *= texture2D(u_Textures[18], v_TexCoord); break;
		case 19: texColor *= texture2D(u_Textures[19], v_TexCoord); break;
		case 20: texColor *= texture2D(u_Textures[20], v_TexCoord); break;
		case 21: texColor *= texture2D(u_Textures[21], v_TexCoord); break;
		case 22: texColor *= texture2D(u_Textures[22], v_TexCoord); break;
		case 23: texColor *= texture2D(u_Textures[23], v_TexCoord); break;
		case 24: texColor *= texture2D(u_Textures[24], v_TexCoord); break;
		case 25: texColor *= texture2D(u_Textures[25], v_TexCoord); break;
		case 26: texColor *= texture2D(u_Textures[26], v_TexCoord); break;
		case 27: texColor *= texture2D(u_Textures[27], v_TexCoord); break;
		case 28: texColor *= texture2D(u_Textures[28], v_TexCoord); break;
		case 29: texColor *= texture2D(u_Textures[29], v_TexCoord); break;
		case 30: texColor *= texture2D(u_Textures[30], v_TexCoord); break;
		case 31: texColor *= texture2D(u_Textures[31], v_TexCoord); break;
	}

    if (texColor.a == 0.0)
		discard;


	for(int i = 0; i < MAX_POINT_LIGHT_COUNT; i++)
	{
		result += CalcPointLight(u_PointLight[i], texColor);
	}

	FragColor = vec4(result, 1.0);
	EntityID  = v_EntityID;
}
