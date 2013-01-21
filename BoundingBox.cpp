#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	this->bboxPos=vec3(0.0f);
	this->bboxSide=vec3(0.0f);
	this->vaoh=0;
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::setBboxPos(vec3 p)
{
	this->bboxPos=p;
}
void BoundingBox::setBboxSide(vec3 s)
{
	this->bboxSide=s;
}
vec3 BoundingBox::getBboxPos()
{
	return this->bboxPos;
}
vec3 BoundingBox::getBboxSide()
{
	return this->bboxSide;
}

GLuint BoundingBox::getVaoh()
{
	return this->vaoh;
}

void BoundingBox::setVaoh(GLuint vaoh)
{
	this->vaoh=vaoh;
}
