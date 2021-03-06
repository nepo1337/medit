#ifndef INTERSECTION_H
#define INTERSECTION_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>

using namespace std;
using namespace glm;

class Intersection
{
public:
	Intersection();
	~Intersection();
	vec3 rayIntersectTriangle(vec3 origin, vec3 direction, vec3 vert1, vec3 vert2, vec3 vert3);
	vec3 getClickRay(int x,int y,mat4 viewM,mat4 projM,int wwidth, int wheight,vec3 pos);

};

#endif // INTERSECTION_H
