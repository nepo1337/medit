#ifndef MODEL_H
#define MODEL_H
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "MeshInfo.h"

using namespace std;
using namespace glm;

class Model
{
private:
	vec3 pos;
	vector<MeshInfo>* meshInfo;
public:
	Model();
	~Model();
	void setMesh(vector<MeshInfo> *m);

};

#endif // MODEL_H
