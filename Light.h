#ifndef LIGHT_H
#define LIGHT_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>
#include <vector>
#include "MeshInfo.h"
#include "BoundingBox.h"

using namespace std;
using namespace glm;

class Light
{
private:
	vec3 pos;
	vector<MeshInfo>* meshInfo;
	BoundingBox* bbox;
	mat4 modelMatrix;
	mat3 normalMatrix;
	void updateModelMatrix();
	float rotx,roty,rotz;
	bool selected;
	string meshName;
	string type;
	int id;
	vec3 color;
	float radius;
public:
	Light();
	~Light();
	void setMesh(vector<MeshInfo> *m);
	vector<MeshInfo>* getMeshInfo();
	void setPos(vec3 pos);
	vec3 getPos();
	mat4 getModelMatrix();
	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);
	vec3 getRot();
	void setBoundingBox(BoundingBox *b);
	BoundingBox* getBoundingBox();
	void select();
	void unSelect();
	bool isSelected();
	void setMeshName(string name);
	string getMeshName();
	void bindId(int id);
	int getId();
	void setColor(vec3 color);
	void setRadius(float rad);
	vec3 getColor();
	float getRadius();
};

#endif // LIGHT_H
