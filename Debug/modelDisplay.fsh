#version 400

out vec4 fragColor;
in vec3 normal;

in vec2 texCoord;

uniform sampler2D tex1;

void main()
{
  vec4 texColor1 = texture(tex1,texCoord);
  
  vec3 ka = vec3(0.5,0.5,0.5);
  vec3 kd = vec3(0.7,0.7,0.7);
  vec3 n = normalize(normal);
 vec4 lightPos = vec4(1,1,1,0);
  vec3 s = normalize(vec3(lightPos));
  
  
  vec3 r = reflect(-s,n);
  vec3 ambdiff=(ka + kd*max(dot(s,n),0.0));
  
  fragColor=vec4(ambdiff,1.0)*texColor1;
}
