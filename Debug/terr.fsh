#version 400

out vec4 fragColor;
in vec3 normal;
in vec3 position;
in vec2 texCoord;
in vec2 bmTexCoord;

uniform sampler2D blendmap1;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float test;

void main()
{
  vec4 blendColors = texture(blendmap1,bmTexCoord);
  vec4 texColor = texture(tex1,texCoord)*blendColors.r;
  texColor += texture(tex2,texCoord)*blendColors.g;

  
  vec3 ka = vec3(0.2,0.2,0.2);
  vec3 kd = vec3(0.7,0.7,0.7);
  vec3 n = normalize(normal);
  vec3 s;
  vec4 lightPos = vec4(1,10,1,0);
  vec3 lightIntens = vec3(0.8,0.8,0.8);

  s = normalize(vec3(lightPos));
  
  vec3 r = reflect(-s,n);
  vec3 ambdiff=lightIntens*(ka + kd*max(dot(s,n),0.0));
  
  fragColor=vec4(ambdiff,1.0)*texColor;
}
