#include "MeshInfo.h"

MeshInfo::MeshInfo()
{
	this->texH=0;
	this->vaoh=0;
	this->nrOfVerts=0;
}

MeshInfo::~MeshInfo()
{
}

GLuint MeshInfo::getTexh()
{
	return this->texH;
}
GLuint MeshInfo::getVaoh()
{
	return this->vaoh;
}

void MeshInfo::setTexh(GLuint h)
{
	this->texH=h;
}

void MeshInfo::setVaoh(GLuint h)
{
	this->vaoh=h;
}
void MeshInfo::setNrOfVerts(int n)
{
	this->nrOfVerts=n;
}
int MeshInfo::getNrOfVerts()
{
	return this->nrOfVerts;
}