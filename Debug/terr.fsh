#version 400

out vec4 fragColor;
in vec3 normal;
in vec3 position;
in vec2 texCoord;
in vec2 bmTexCoord;

uniform sampler2D gridMap;
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
uniform bool showGM;

void main()
{
	vec4 gridColors = texture(gridMap,bmTexCoord);
	vec4 texColor;

	vec4 blendColors1 = texture(blendmap1,bmTexCoord);
	vec4 blendColors2 = texture(blendmap2,bmTexCoord);
		
	//first blendmap
	texColor = texture(tex1,texCoord)*blendColors1.r;
	texColor += texture(tex2,texCoord)*blendColors1.g;
	texColor += texture(tex3,texCoord)*blendColors1.b;
	texColor += texture(tex4,texCoord)*blendColors1.a;
			
	//2nd blendmap
	texColor += texture(tex5,texCoord)*blendColors2.r;
	texColor += texture(tex6,texCoord)*blendColors2.g;
	texColor += texture(tex7,texCoord)*blendColors2.b;
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
	
	if(showGM==true)
	{
		if(gridColors.a>0.0)
		{
			texColor=vec4(1,0,0,1);
		}
	}
  
	fragColor=vec4(ambdiff,1.0)*texColor;
}
