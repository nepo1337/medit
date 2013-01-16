#ifndef SURFACETEX_H
#define SURFACETEX_H

#define GLEW_STATIC
#include <glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "SOIL/SOIL.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

class SurfaceTex
{
private:
	GLuint texHandle;
	GLuint vaoh;
	GLuint vbohs[3];
	vector<float> rotations;
	vector<vec3> positions;
	string name;
public:
	SurfaceTex();
	~SurfaceTex();
	void init(string texName);
	GLuint getTexHandle();
	GLuint getVaoH();
	vector<float>* getRotations();
	vector<vec3>* getPositions();
	void addSurface(float rot, vec3 pos);
	string getName();

};

#endif // SURFACETEX_H