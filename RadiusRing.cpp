#include "RadiusRing.h"

#define PI 3.14159265

RadiusRing::RadiusRing()
{
	this->scale=1.0f;
	this->pos=vec3(1.0f);
	this->nrOfLines=20;
}

RadiusRing::~RadiusRing()
{
}

void RadiusRing::init(int nrOfLines)
{
	vector<float> points;
	this->nrOfLines=nrOfLines;
	for(int i=0;i<nrOfLines;i++)
	{
		points.push_back(cos((PI*i*2)/nrOfLines));
		points.push_back(0.0);
		points.push_back(sin((PI*i*2)/nrOfLines));
	}
	//the last point, so the circle ends
	points.push_back(1.0f);
	points.push_back(0.0f);
	points.push_back(0.0f);
	this->nrOfLines+=1;
	
	glGenVertexArrays(1,&this->vaoh);
	glGenBuffers(1,&this->vboH);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*points.size(),&points[0],GL_STATIC_DRAW);
	
	glBindVertexArray(this->vaoh);
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboH);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);

}
void RadiusRing::free()
{
	glDeleteBuffers(1,&this->vboH);
	glDeleteVertexArrays(1, &this->vaoh);
}

void RadiusRing::setPos(vec3 pos)
{
	this->pos=pos;
}
vec3 RadiusRing::getPos()
{
	return this->pos;
}
GLuint RadiusRing::getVaoh()
{
	return this->vaoh;
}

int RadiusRing::getNrOfLines()
{
	return this->nrOfLines;
}
float RadiusRing::getScale()
{
	return this->scale;
}
void RadiusRing::setScale(float f)
{
	this->scale=f;
}