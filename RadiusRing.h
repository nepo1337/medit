#ifndef RADIUSRING_H
#define RADIUSRING_H

#define GLEW_STATIC
#include <glew.h>
#include <cmath>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>

using namespace glm;
using namespace std;

class RadiusRing
{
private:
	vec3 pos;
	float scale;
	GLuint vaoh;
	GLuint vboH;
	int nrOfLines;
public:
	RadiusRing();
	~RadiusRing();
	void setPos(vec3 pos);
	void setScale(float f);
	float getScale();
	vec3 getPos();
	void init(int nrOfLines);
	void free();
	GLuint getVaoh();
	int getNrOfLines();
};

#endif // RADIUSRING_H
