#version 400

in vec3 vertexPosition;
in vec2 vertexUv;

out vec2 uv;

void main()
{
  uv=vertexUv;
  gl_Position = vec4(vertexPosition,1.0);
}
