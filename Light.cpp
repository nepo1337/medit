#include "Light.h"


Light::Light()
{
	this->pos=vec3(0);
	this->meshInfo=0;
	
	this->modelMatrix=mat4(1.0f);
	this->normalMatrix=mat3(1.0f);
	rotx=roty=rotz=0;
	this->selected=false;
	this->meshName="";
	this->id=0;
	this->radius=0;
}

Light::~Light()
{
}

void Light::setMesh(vector<MeshInfo> *m)
{
	this->meshInfo=m;
}

void Light::updateModelMatrix()
{
	this->modelMatrix=mat4(1.0f);
	this->modelMatrix*=translate(this->pos);
	this->modelMatrix*=rotate(this->rotx,glm::vec3(1.0f,0.0f,0.0f));
	this->modelMatrix*=rotate(this->roty,glm::vec3(0.0f,1.0f,0.0f));
	this->modelMatrix*=rotate(this->rotz,glm::vec3(0.0f,0.0f,1.0f));
}

mat4 Light::getModelMatrix()
{
	return this->modelMatrix;
}

void Light::setPos(vec3 pos)
{
	this->pos=pos;
	this->updateModelMatrix();
}

vec3 Light::getPos()
{
	return this->pos;
}
vector<MeshInfo>* Light::getMeshInfo()
{
	return this->meshInfo;
}

void Light::setBoundingBox(BoundingBox *b)
{
	this->bbox=b;
}

BoundingBox* Light::getBoundingBox()
{
	return this->bbox;
}

void Light::rotateX(float x)
{
	this->rotx=x;
	this->updateModelMatrix();
}

void Light::rotateY(float y)
{
	this->roty=y;
	this->updateModelMatrix();
}

void Light::rotateZ(float z)
{
	this->rotz=z;
	this->updateModelMatrix();
}

void Light::select()
{
	this->selected=true;
}
void Light::unSelect()
{
	this->selected=false;
}
bool Light::isSelected()
{
	return this->selected;
}
void Light::setMeshName(string name)
{
	this->meshName=name;
}
string Light::getMeshName()
{
	return this->meshName;
}
vec3 Light::getRot()
{
	return vec3(this->rotx,this->roty,this->rotz);
}


void Light::bindId(int id)
{
	this->id=id;
}
int Light::getId()
{
	return this->id;
}
void Light::setColor(vec3 col)
{
	this->color=col;
}
void Light::setRadius(float rad)
{
	this->radius=rad;
}

vec3 Light::getColor()
{
	return this->color;
}

float Light::getRadius()
{
	return this->radius;
}