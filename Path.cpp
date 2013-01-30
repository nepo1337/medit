#include "Path.h"

Path::Path()
{
	this->vaoh=0;
	this->vboh=0;
	this->selected=false;
}

Path::~Path()
{
}
void Path::freeGFX()
{
	glDeleteBuffers(1,&this->vboh);
	glDeleteVertexArrays(1, &this->vaoh);
}

void Path::addFlag(vec3 pos)
{
	this->flagPositions.push_back(pos);
	if(this->vaoh!=0)
	{
		glDeleteBuffers(1,&this->vboh);
		glDeleteVertexArrays(1, &this->vaoh);
	}

	vector<float> verts;
	for(unsigned int i=0;i<this->flagPositions.size();i++)
	{
		verts.push_back(this->flagPositions[i].x);
		verts.push_back(this->flagPositions[i].y);
		verts.push_back(this->flagPositions[i].z);
	}
	
	glGenVertexArrays(1,&this->vaoh);
	glGenBuffers(1,&this->vboh);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vboh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*verts.size(),&verts[0],GL_STATIC_DRAW);
	
	glBindVertexArray(this->vaoh);
	glEnableVertexAttribArray(3);
	
	//vertex
	glBindBuffer(GL_ARRAY_BUFFER, this->vboh);
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);
}

vec3 Path::getFlagPosition(int i)
{
	if(i>=0&&i<this->flagPositions.size())
		return this->flagPositions[i];
	return vec3(-1);
}

int Path::getNrOfFlags()
{
	return this->flagPositions.size();
}

void Path::setColor(vec3 c)
{
	this->color=c;
}

vec3 Path::getColor()
{
	return this->color;
}
GLuint Path::getVaohToLines()
{
	return this->vaoh;
}

void Path::select()
{
	this->selected=true;
}

void Path::unselect()
{
	this->selected=false;
}
bool Path::isSelected()
{
	return this->selected;
}