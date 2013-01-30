#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 normal;
out vec3 position;

uniform mat3 normalMatrix;
uniform mat4 MVP;

void main()
{ 
  normal = normalize(normalMatrix*vertexNormal);
  gl_Position = MVP*vec4(vertexPosition,1.0);
}
