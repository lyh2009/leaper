#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;

uniform vec3 u_ViewPos;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform int u_EntityID;

uniform vec3 lightPos = vec3(1.2f, -3.0f, -2.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main() {

  vec3 FragPos = v_Position;

  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor * texture(u_Diffuse, v_TexCoord).rgb;

  // diffuse
  vec3 norm = normalize(v_Normal);
  vec3 lightDir = normalize(lightPos - v_Position);
  float diff = max(dot(norm, lightDir), 0.0) * 0.5 + 0.5;
  vec3 diffuse = diff * lightColor * texture(u_Diffuse, v_TexCoord).rgb;

  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(u_ViewPos + lightDir);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor * texture(u_Specular, v_TexCoord).rgb;

  vec3 result = (ambient + diffuse + specular);
  FragColor = vec4(result, 1.0);
  EntityID = u_EntityID;
}
