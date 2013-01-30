#version 400

out vec4 fragColor;
in vec3 normal;
uniform float outAlpha;
uniform float ro;
uniform float go;
uniform float bo;

void main()
{  
  vec3 ka = vec3(0.2,0.2,0.2);
  vec3 kd = vec3(0.7,0.7,0.7);
  vec3 n = normalize(normal);
  vec3 s;
  vec4 lightPos = vec4(1,10,1,0);
  vec3 lightIntens = vec3(0.8,0.8,0.8);
  
  s = normalize(vec3(lightPos));
  
  vec3 r = reflect(-s,n);
  vec3 ambdiff=lightIntens*(ka + kd*max(dot(s,n),0.0));
 
  
  fragColor=vec4(ambdiff*vec3(ro,go,bo),outAlpha);
}
