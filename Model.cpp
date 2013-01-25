#include "Model.h"

Model::Model()
{
	this->pos=vec3(0);
	this->meshInfo=0;
	
	this->modelMatrix=mat4(1.0f);
	this->normalMatrix=mat3(1.0f);
	rotx=roty=rotz=0;
	this->scale=1.0f;
	this->selected=false;
	this->meshName="";
}

Model::~Model()
{
}

void Model::setMesh(vector<MeshInfo> *m)
{
	this->meshInfo=m;
}

void Model::updateModelMatrix()
{
	this->modelMatrix=mat4(1.0f);
	this->modelMatrix*=translate(this->pos);
	this->modelMatrix*=rotate(this->rotx,glm::vec3(1.0f,0.0f,0.0f));
	this->modelMatrix*=rotate(this->roty,glm::vec3(0.0f,1.0f,0.0f));
	this->modelMatrix*=rotate(this->rotz,glm::vec3(0.0f,0.0f,1.0f));
	this->modelMatrix*=glm::scale(mat4(1.0f),vec3(this->scale));
}

mat4 Model::getModelMatrix()
{
	return this->modelMatrix;
}

void Model::setPos(vec3 pos)
{
	this->pos=pos;
	this->updateModelMatrix();
}

vec3 Model::getPos()
{
	return this->pos;
}
vector<MeshInfo>* Model::getMeshInfo()
{
	return this->meshInfo;
}

void Model::setBoundingBox(BoundingBox *b)
{
	this->bbox=b;
}

BoundingBox* Model::getBoundingBox()
{
	return this->bbox;
}

void Model::rotateX(float x)
{
	this->rotx=x;
	this->updateModelMatrix();
}

void Model::rotateY(float y)
{
	this->roty=y;
	this->updateModelMatrix();
}

void Model::rotateZ(float z)
{
	this->rotz=z;
	this->updateModelMatrix();
}
void Model::scaleXYZ(float f)
{
	this->scale=f;
	this->updateModelMatrix();
}

void Model::select()
{
	this->selected=true;
}
void Model::unSelect()
{
	this->selected=false;
}
bool Model::isSelected()
{
	return this->selected;
}
void Model::setMeshName(string name)
{
	this->meshName=name;
}
string Model::getMeshName()
{
	return this->meshName;
}
vec3 Model::getRot()
{
	return vec3(this->rotx,this->roty,this->rotz);
}

float Model::getScale()
{
	return this->scale;
}