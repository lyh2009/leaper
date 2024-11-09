#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec4 v_FragPosLightSpace;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewContentBroswerion;
    vec3 u_ViewPos;
};

uniform mat4 u_Trans;
uniform mat4 lightSpaceMat;

void main()
{
    v_Normal = mat3(transpose(inverse(u_Trans))) * a_Normal;

    v_Position          = vec3(u_Trans * vec4(a_Position, 1.0));
    v_TexCoord          = a_TexCoord;
    v_FragPosLightSpace = lightSpaceMat * vec4(v_Position, 1.0);

    gl_Position = u_ViewContentBroswerion * u_Trans * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec4 v_FragPosLightSpace;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewContentBroswerion;
    vec3 u_ViewPos;
};

layout(std140, binding = 1) uniform Light
{
    vec3 lightColor;
    float ambientStrength;
    float specularStrength;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_POINT_LIGHTS 16
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_PointLightsSize;

uniform vec3 lightPos;

layout(binding = 0) uniform sampler2D u_ShadowMap;
layout(binding = 1) uniform sampler2D u_Diffuse;
layout(binding = 2) uniform sampler2D u_Specular;
uniform int u_EntityID;

float ShadowCalculation(vec4 fragPosLightSpace, float _dot)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias     = max((0.05 * (1.0 - _dot)), 0.005);
    float shadow   = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1)
        shadow = 0;

    return shadow;
}

float ClcPointLight(PointLight light)
{
    float d = length(light.position - v_Position);
    float a = 1.0 / (light.constant + light.linear * d + light.quadratic * d * d);
    return a;
}

void main()
{
    vec3 color = texture(u_Diffuse, v_TexCoord).rgb;

    // ambient

    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm     = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_Position);
    float diff    = max(dot(norm, lightDir), 0.0) * 0.5 + 0.5;
    vec3 diffuse  = diff * lightColor;

    // specular

    vec3 viewDir    = normalize(u_ViewPos + lightDir);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular   = specularStrength * spec * lightColor * texture(u_Specular, v_TexCoord).rgb;
    float a         = 1.0;
    for (int i = 0; i < u_PointLightsSize; ++i)
    {
        a = ClcPointLight(u_PointLights[i]);
        ambient += a;
        diffuse += a;
        specular += a;
    }

    float shadow  = ShadowCalculation(v_FragPosLightSpace, dot(norm, lightDir));
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * color;
    FragColor     = vec4(lighting, 1.0);
    EntityID      = u_EntityID;
}
