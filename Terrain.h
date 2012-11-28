#ifndef TERRAIN_H
#define TERRAIN_H
#define GLEW_STATIC
#include <glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <SFML/Graphics.hpp>
#include "SOIL/SOIL.h"
#include "TA.h"
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
	TerrainInfo terrInf;
public:
	Terrain(float width, float height, int texScale);
	~Terrain();
	GLuint getVaoh();
	GLuint getTexh();
	TerrainInfo *getTerrInfo();

};

#endif // TERRAIN_H
