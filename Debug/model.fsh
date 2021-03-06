#version 400

out vec4 fragColor;
in vec3 normal;
in vec3 position;
in vec2 texCoord;

uniform sampler2D tex1;
uniform float outAlpha;

void main()
{
  vec4 texColor1 = texture(tex1,texCoord);
  
  vec3 ka = vec3(0.5,0.5,0.5);
  vec3 kd = vec3(0.7,0.7,0.7);
  vec3 n = normalize(normal);
  vec3 s;
  vec4 lightPos = vec4(1,10,1,0);
  vec3 lightIntens = vec3(1.0,1.0,1.0);
  
 // if(lightPos.w==0)
 // {
    s = normalize(vec3(lightPos));
 // }
 // else
 // {
 //   s = normalize(vec3(lightPos)-position);
 // }
  
  vec3 r = reflect(-s,n);
  vec3 ambdiff=lightIntens*(ka + kd*max(dot(s,n),0.0));
  
  fragColor=vec4(ambdiff,outAlpha)*texColor1;
}
