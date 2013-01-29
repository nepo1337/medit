#include "Path.h"

Path::Path()
{
}

Path::~Path()
{
}

void Path::addFlag(vec3 pos)
{
	this->flagPositions.push_back(pos);
}

vec3 Path::getFlagPosition(int i)
{
	if(i>=0&&i<this->flagPositions.size())
		return this->getFlagPosition(i);
	return vec3(-1);
}

int Path::getNrOfFlags()
{
	return this->flagPositions.size();
}