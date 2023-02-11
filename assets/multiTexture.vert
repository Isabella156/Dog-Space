#version 430
layout(location = 0) in vec3 iPosition;
layout(location = 1) in vec3 iNormal;
layout(location = 2) in vec3 iColor;
layout(location = 3) in vec2 iTexCoord;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;
layout(location = 6) uniform mat3 normal;

out vec3 v2fNormal;
out vec3 v2fPos;
out vec3 v2fColor;
out vec2 v2fTexCoord;
void main()
{
  gl_Position = projection * view * model * vec4(iPosition, 1.0);
  v2fNormal = normal * iNormal;
  v2fPos = vec3(model * vec4(iPosition, 1.0));
  v2fColor = iColor;
  v2fTexCoord = iTexCoord;
}
