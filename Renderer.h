#ifndef RENDERER_H
#define RENDERER_H
#define GLEW_STATIC
#include <glew.h>
#include <iostream>
#include "Model.h"
#include "GLSLProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <vector>
#include "MeshInfo.h"
#include "TA.h"

using namespace std;
using namespace glm;
class Renderer
{
private:
	GLSLProgram modelShader;
	GLSLProgram TerrainShader;
	mat4 viewMatrix,projMatrix;
	int wwidth,wheight;
	bool debug;
	vector<Model*> models;
	TerrainInfo *terrInf;
public:
	Renderer();
	~Renderer();
	void draw();
	void updateCamera(vec3 pos);
	void updateProjMatrix(float width, float height);
	void addModel(Model* m);
	void setTerrainInfo(TerrainInfo *t);
	vec3 getClickRay(int x,int y);
};

#endif // RENDERER_H