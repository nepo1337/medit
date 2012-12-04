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
	float radius;
	TerrainInfo terrInf;
	sf::Image blendmap1;
	sf::Image blendmap2;
	//handle gets filles with a handle to the tex in the GFX, uploads data from sf::Image
	void makeBlendMap(GLuint& handle, sf::Image img);
	
	//0,1,2
	int mapsize;
	//modifiner for scaling blendmap
	int blendsc;
	void increasePixelPaint(sf::Color &pix1, sf::Color &pix2,int blendIndex,float strength);
	bool inCircle(float cx, float cy, float x, float y);
	void replacePartTexture(GLint xoffset, GLint yoffset, sf::Image img,GLuint handle);
	
public:
	//the terrain has predefined sizes,0 small, 1 medium, 2 large
	Terrain(int size);
	~Terrain();
	GLuint getVaoh();
	GLuint getTexh();
	TerrainInfo *getTerrInfo();
	//the sf::Image should be replaced with an array for ore indexes
	//x and y represents the center,ex where someone clicked
	void paint(int whichTex,vec3 origin, vec3 ray);
	void setRadius(float rad);

};

#endif // TERRAIN_H
