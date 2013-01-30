#ifndef PATHHANDLER_H
#define PATHHANDLER_H
#include <vector>
#include "Path.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <time.h>
#include <iostream>
#include "GLSLProgram.h"
#include "MeshInfo.h"
#include "BoundingBox.h"
#include <fstream>

using namespace glm;
using namespace std;

class PathHandler
{
private:
	vector<Path> Paths;
	GLSLProgram pathRenderer,lineRenderer;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	vector<MeshInfo> *meshInfo;
	BoundingBox * bb;
public:
	PathHandler();
	~PathHandler();
	void addPath();
	void addFlagToCurrentPath(vec3 pos);
	void drawPaths();
	void updateViewMatrix(mat4 view);
	void updateProjectionMatrix(float width, float height);
	void setMesh(vector<MeshInfo> *meshInfo,BoundingBox * bb);
	void init();
	void selectPaths(float normalizedX, float normalizedY,vec3 pos);
	void removeSelectedPaths();
	void save(string path, string filename);
};

#endif // PATHHANDLER_H
