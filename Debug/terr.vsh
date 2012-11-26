#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUv;

out vec3 normal;
out vec3 position;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;

void main()
{ 
  position = vec3(viewMatrix*modelMatrix*vec4(vertexPosition,1.0));
  normal = normalize(vertexNormal);
  texCoord =vertexUv;
  gl_Position = MVP*vec4(vertexPosition,1.0);
}
