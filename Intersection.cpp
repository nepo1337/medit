#include "Intersection.h"

Intersection::Intersection()
{
}

Intersection::~Intersection()
{
}

vec3 Intersection::rayIntersectTriangle(vec3 origin, vec3 direction, vec3 vert1, vec3 vert2, vec3 vert3)
{
	bool pass=true;
	bool found=false;
	vec3 hit;
	//for saving the closest target
	float lowestY=999999.0f;

	pass=true;
	vec3 p0=vert1;
	vec3 p1=vert2;
	vec3 p2=vert3;
	
	vec3 e1 = p1-p0;
	vec3 e2 = p2-p0;
	vec3 q = cross(direction,e2);
	float a = dot(e1,q);
	
	if(a>-0.00000001&&a<0.00000001)
	{
		pass=false;
	}
	if(pass)
	{
		float f = 1/a;
		vec3 s = origin-p0;
		float u = f*dot(s,q);
		if(u<0.0)
		{
			pass=false;
		}
		if(pass)
		{
			vec3 r = cross(s,e1);
			float v = f*dot(direction,r);
			if(v<0.0 || u+v>1.0)
			{
				pass=false;
			}
			if(pass)
			{
				float t=f*dot(e2,r);
				if(t<lowestY)
				{
						hit=vec3(v,t,u);
				}
				found=true;
			}
		}
	}
	
	if(!found)
		hit=vec3(1,1,1);
	return hit;
}

vec3 Intersection::getClickRay(int x,int y,mat4 viewM,mat4 projM,int wwidth, int wheight,vec3 pos)
{
	float normalisedx = 2 * (float)x / wwidth - 1;
	float normalisedy = 1 - 2 * (float)y / wheight;
	mat4 unview = inverse(projM*viewM);
	vec4 near_point = unview * vec4(normalisedx, normalisedy, 0, 1);
	//the last vec3 is the pos of the camera
	vec3 t = vec3(near_point.x/near_point.w,near_point.y/near_point.w,near_point.z/near_point.w)-pos;
	vec3 rayd = normalize(t);
	
	return rayd;
}
