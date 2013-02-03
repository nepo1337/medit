#version 400

in vec3 vertexPosition;
in vec2 vertexUv;

out vec2 texCoord;

uniform mat4 MVP;

void main()
{ 
  texCoord =vertexUv;
  gl_Position = MVP*vec4(vertexPosition,1.0);
}
