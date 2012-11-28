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
