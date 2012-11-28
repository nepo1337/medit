#version 400

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUv;
in vec2 vertexUvBM;

out vec3 normal;
out vec3 position;
out vec2 texCoord;
out vec2 bmTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;

void main()
{ 
  position = vec3(viewMatrix*modelMatrix*vec4(vertexPosition,1.0));
  normal = normalize(vertexNormal);
  texCoord =vertexUv;
  bmTexCoord=vertexUvBM;
  gl_Position = MVP*vec4(vertexPosition,1.0);
}
