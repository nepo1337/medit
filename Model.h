#ifndef MODEL_H
#define MODEL_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>
#include <vector>
#include "MeshInfo.h"
#include "BoundingBox.h"

using namespace std;
using namespace glm;

class Model
{
private:
	vec3 pos;
	vector<MeshInfo>* meshInfo;
	BoundingBox* bbox;
	mat4 modelMatrix;
	mat3 normalMatrix;
	void updateModelMatrix();
	float rotx,roty,rotz,scale;
	bool selected;
public:
	Model();
	~Model();
	void setMesh(vector<MeshInfo> *m);
	vector<MeshInfo>* getMeshInfo();
	void setPos(vec3 pos);
	vec3 getPos();
	mat4 getModelMatrix();
	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);
	void scaleXYZ(float f);
	void setBoundingBox(BoundingBox *b);
	BoundingBox* getBoundingBox();
	void select();
	void unSelect();
	bool isSelected();

};

#endif // MODEL_H
