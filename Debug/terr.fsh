#version 400

out vec4 fragColor;
in vec3 normal;
in vec3 position;
in vec2 texCoord;
in vec2 bmTexCoord;

uniform sampler2D blendmap1;
uniform sampler2D blendmap2;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;
uniform float test;

void main()
{
	vec4 blendColors1 = texture(blendmap1,bmTexCoord);
	vec4 blendColors2 = texture(blendmap2,bmTexCoord);
	vec4 texColor;
	
	//first blendmap
	if(blendColors1.r>0)
		texColor = texture(tex1,texCoord)*blendColors1.r;
	if(blendColors1.g>0)	
		texColor += texture(tex2,texCoord)*blendColors1.g;
	if(blendColors1.b>0)
		texColor += texture(tex3,texCoord)*blendColors1.b;
	if(blendColors1.a>0)
		texColor += texture(tex4,texCoord)*blendColors1.a;
		
	//2nd blendmap
	if(blendColors2.r>0)
		texColor += texture(tex5,texCoord)*blendColors2.r;
	if(blendColors2.g>0)	
		texColor += texture(tex6,texCoord)*blendColors2.g;
	if(blendColors2.b>0)
		texColor += texture(tex7,texCoord)*blendColors2.b;
	if(blendColors2.a>0)
		texColor += texture(tex8,texCoord)*blendColors2.a;

  
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
