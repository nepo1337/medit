#version 400

in vec3 vertexPosition;
in vec2 vertexUv;

uniform mat4 modelMatrix;

out vec2 uv;

void main()
{
  uv=vertexUv;
  gl_Position = modelMatrix*vec4(vertexPosition,1.0);
}
