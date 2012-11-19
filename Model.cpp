#include "Model.h"

Model::Model()
{
	this->pos=vec3(0);
	this->meshInfo=0;
}

Model::~Model()
{
}

void Model::setMesh(vector<MeshInfo> *m)
{
	this->meshInfo=m;
}