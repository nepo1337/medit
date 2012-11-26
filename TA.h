#ifndef TA_H
#define TA_H
#define GLEW_STATIC
#include <glew.h>

//a include file for the terrain info struct
//dont want a class for getting/setting lots of handels

struct TerrainInfo
{
	GLuint vaoh,texH,texB;
	GLuint vbohs[3];
};

#endif