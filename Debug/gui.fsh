#version 400

out vec4 fragColor;
in vec2 uv;

uniform sampler2D guiTex;
void main()
{
  vec4 texColor=texture(guiTex,uv);
  //if(texColor.a==0.0)
    //discard;
  fragColor=texColor;
}
