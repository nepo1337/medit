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
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Intersection.h>

using namespace std;
using namespace glm;

class Terrain
{
private:
	Intersection intersect;
	float width,height;
	GLuint vaoh;
	GLuint vbohs[4];
	bool debug;
	TerrainInfo terrInf;
	sf::Image blendmap1;
	//handle gets filles with a handle to the tex in the GFX, uploads data from sf::Image
	void makeBlendMap(GLuint& handle, sf::Image img);
public:
	Terrain(float width, float height);
	~Terrain();
	GLuint getVaoh();
	GLuint getTexh();
	TerrainInfo *getTerrInfo();
	//the sf::Image should be replaced with an array for ore indexes
	//x and y represents the center,ex where someone clicked
	void paint(int indexForblendMap,float radius,int whichTex,vec3 origin, vec3 ray);

};

#endif // TERRAIN_H
