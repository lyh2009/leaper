#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in flat int v_EntityID;
in vec3 v_Normal;

layout(binding = 0) uniform sampler2D u_Textures[32];

uniform vec3 lightPos = vec3(1.2f, -3.0f, -2.0f);
uniform vec3 u_ViewPosition;
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main() {
  vec3 objectColor = vec3(1.0f, 0.5f, 0.3f);

  vec3 FragPos = v_Position;
  vec3 viewPos = u_ViewPosition;

  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(v_Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0) * 0.5 + 0.5;
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos + lightDir);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
  EntityID = v_EntityID;
}
