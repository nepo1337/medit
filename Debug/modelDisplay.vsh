#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUv;

out vec3 normal;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main()
{ 
  normal = normalize(normalMatrix*vertexNormal);
  texCoord =vertexUv;
  gl_Position = modelMatrix*vec4(vertexPosition,1.0);
}
