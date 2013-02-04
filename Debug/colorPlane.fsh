#version 400

out vec4 fragColor;

uniform float ro;
uniform float go;
uniform float bo;
void main()
{
  fragColor=vec4(ro,go,bo,1);
}
