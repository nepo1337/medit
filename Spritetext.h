#ifndef SPRITETEXT_H
#define SPRITETEXT_H

#define GLEW_STATIC
#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <glew.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;
using namespace glm;

class Spritetext
{
private:
	string text;
	float x,y,size;
	int posX,posY;
	GLuint texHandle,vaoHandle;
	GLuint vboH[2];
	sf::Image spriteFile;
	int nrOfVerts;
public:
	string getText();
	int getXPos();
	int getYPos();
	void setSize(float size);
	float getSize();
	Spritetext(string text, int posX, int posY,float size,int screenResX,int screenResY,string spriteFile,int charWidth,int charHeight);
	Spritetext();
	~Spritetext();
	
	GLuint getVaoHandle();
	GLuint getTexHandle();
	int getNrOfVerts();

};

#endif // SPRITETEXT_H
