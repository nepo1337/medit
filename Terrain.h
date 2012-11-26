#ifndef TERRAIN_H
#define TERRAIN_H
#define GLEW_STATIC
#include <glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "SOIL/SOIL.h"
#include <iostream>

using namespace std;
using namespace glm;

class Terrain
{
private:
	float width,height;
	int texScale;
	GLuint vaoh,texH;
	GLuint vbohs[3];
	bool debug;
public:
	Terrain(float width, float height, int texScale);
	~Terrain();
	GLuint getVaoh();

};

#endif // TERRAIN_H
