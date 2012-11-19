#include "Object.h"
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;
using namespace std;
Object::Object()
{
    this->nrVerts=0;
    this->ka=vec3(0);
    this->kd=vec3(0);
    this->ks=vec3(0);
    this->ns=0;
    this->tr=0;
    this->textrueKa="";
    this->textureKd="";
}

Object::~Object()
{

}
vector< float > Object::getNormalArray()
{
    return this->normalArray;
}
vector< float > Object::getVertexArray()
{
    return this->vertexArray;
}
vector< float > Object::getUvArray()
{
    return this->uvArray;
}



float Object::getTr()
{
    return this->tr;
}
vec3 Object::getKa()
{
    return this->ka;
}
vec3 Object::getKd()
{
    return this->kd;
}
vec3 Object::getKs()
{
    return this->ks;
}
int Object::getNrOfVerts()
{
    return this->vertexArray.size()/3;
}
int Object::getNrOfNormals()
{
	return this->normalArray.size()/3;
}
int Object::getNrOfUvs()
{
	return this->uvArray.size()/2;
}
float Object::getNs()
{
    return this->ns;
}
string Object::getTextureNameKa()
{
    return this->textrueKa;
}
string Object::getTextureNameKd()
{
    return this->textureKd;
}

void Object::setTr(float d)
{
    this->tr=d;
}
void Object::setKa(vec3 ka)
{
    this->ka=ka;
}
void Object::setKd(vec3 kd)
{
    this->kd=kd;
}
void Object::setKs(vec3 ks)
{
    this->ks=ks;
}

void Object::setNs(float ns)
{
    this->ns=ns;
}
void Object::setTextureNameKa(string s)
{
    this->textrueKa=s;
}
void Object::setTextureNameKd(string s)
{
    this->textureKd=s;
}
string Object::getUsemtl()
{
    return this->usemtl;
}
void Object::setUsemtl(string s)
{
    this->usemtl=s;
}

void Object::addNormal(vec3 n)
{
    this->normalArray.push_back(n.x);
    this->normalArray.push_back(n.y);
    this->normalArray.push_back(n.z);
}
void Object::addUv(vec2 uv)
{
    this->uvArray.push_back(uv.x);
    this->uvArray.push_back(uv.y);
}
void Object::addVertex(vec3 v)
{
    this->vertexArray.push_back(v.x);
    this->vertexArray.push_back(v.y);
    this->vertexArray.push_back(v.z);
}

void Object::print()
{
    for(int i=0; i<vertexArray.size(); i++)
    {
        cout << vertexArray[i]<<endl;
    }
    cout << vertexArray.size()<<endl;
}
