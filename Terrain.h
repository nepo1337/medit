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
#include "GLSLProgram.h"

using namespace std;
using namespace glm;

class Terrain
{
private:
	GLSLProgram TerrainShader;
	Intersection intersect;
	float width,height;
	GLuint vaoh, gridTexHandle;
	GLuint vbohs[4];
	bool debug;
	int radius;
	TerrainInfo terrInf;
	sf::Image blendmap1;
	sf::Image blendmap2;
	sf::Image gridMap;
	sf::Image minimap;
	sf::Color tex1Avg,tex2Avg,tex3Avg,tex4Avg,tex5Avg,tex6Avg,tex7Avg,tex8Avg;
	
	//textures
	sf::Image tex1,tex2,tex3,tex4,tex5,tex6,tex7,tex8;
	//handle gets filles with a handle to the tex in the GFX, uploads data from sf::Image
	void makeBlendMap(GLuint& handle, sf::Image img);
	
	//instead of using soil, witch takes the picture, uploads it, then removes the data from the ram
	//ill use a own function so the data is still left, used to look up colors in the minimap
	GLuint uploadTextureGFX(sf::Image img);
	
	//0,1,2
	int mapsize;
	//modifiner for scaling blendmap
	int blendsc;
	//the distance is for calculating the dropoff
	void increasePixelPaint(sf::Color &pix1, sf::Color &pix2,int blendIndex,float distance);
	bool inCircle(float cx, float cy, float x, float y);
	void replacePartTexture(GLint xoffset, GLint yoffset, sf::Image img,GLuint handle);
	
	float activeTex,opacity,dropoff;
	void makeMiniMap();
	sf::Color getSample(int x, int y,float xratio,float yratio);
	sf::Color getAverageTexColor(sf::Image &img);
	void swapImg(sf::Image &img);
	mat4 viewMatrix;
	mat4 projMatrix;
public:
	//the terrain has predefined sizes,0 small, 1 medium, 2 large
	Terrain(int size);
	~Terrain();
	TerrainInfo *getTerrInfo();
	//the sf::Image should be replaced with an array for ore indexes
	//x and y represents the center,ex where someone clicked
	void paint(vec3 origin, vec3 ray);
	void setRadius(float rad);
	void setActiveTex(int tex);
	void setOpacity(float opa);
	void setDropoff(float d);
	void saveMaps(string path, string filename);
	int getWidth();
	int getHeight();
	void draw();
	void updateViewMatrix(mat4 viewMatrix);
	void updateProjMatrix(float width, float height);
};

#endif // TERRAIN_H
