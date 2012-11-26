#version 400

out vec4 fragColor;
in vec3 normal;
in vec3 position;
in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float test;

void main()
{
  vec4 texColor1 = texture(tex1,texCoord);
  vec4 texColor2 = texture(tex2,texCoord);
  vec4 texColor;
  
  
	if(texColor2.a<test-0.1)
	{
		texColor=texColor2;
	}
	else if(texColor2.a>test+0.1)
	{
		texColor=texColor1;
	}
	else
	{
		float f=(texColor1.a - test + 0.1) / (2*0.1);
		texColor=mix(texColor1,texColor2,0.5);
	}
  
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
