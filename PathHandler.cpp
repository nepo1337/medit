#include "PathHandler.h"

PathHandler::PathHandler()
{
}

PathHandler::~PathHandler()
{
}

void PathHandler::addPath()
{
	this->Paths.push_back(Path());
}

void PathHandler::addFlagToCurrentPath(vec3 pos)
{
	if(this->Paths.size()>0)
		this->Paths[this->Paths.size()-1].addFlag(pos);
}