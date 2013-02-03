#version 400

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D tex1;

void main()
{
  vec4 texColor1 = texture(tex1,texCoord);
  
  fragColor=texColor1;
}
