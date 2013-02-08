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
protected:
	vec3 pos;
	vector<MeshInfo>* meshInfo;
	BoundingBox* bbox;
	mat4 modelMatrix;
	mat3 normalMatrix;
	void updateModelMatrix();
	float rotx,roty,rotz,scale;
	bool selected;
	string meshName;
	string type;
	int id;
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
	vec3 getRot();
	void scaleXYZ(float f);
	void setBoundingBox(BoundingBox *b);
	BoundingBox* getBoundingBox();
	void select();
	void unSelect();
	bool isSelected();
	void setMeshName(string name);
	string getMeshName();
	float getScale();
	void setType(string type);
	string getType();
	void bindId(int id);
	int getId();
};

#endif // MODEL_H
