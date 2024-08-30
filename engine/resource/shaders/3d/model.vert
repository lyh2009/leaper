#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 u_ProjectionView;
uniform mat4 u_Trans;

void main() {
  v_Normal = mat3(transpose(inverse(u_Trans))) * a_Normal;

  v_Position = vec3(u_Trans * vec4(a_Position, 1.0));
  v_TexCoord = a_TexCoord;

  gl_Position = u_ProjectionView * u_Trans * vec4(a_Position, 1.0);
}