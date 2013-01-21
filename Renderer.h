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
#include "GUI.h"

using namespace std;
using namespace glm;
class Renderer
{
private:
	GLSLProgram modelShader,bBoxShader;
	mat4 viewMatrix,projMatrix;
	int wwidth,wheight;
	bool debug;
	vector<Model*> models;
public:
	Renderer();
	~Renderer();
	void draw();
	void updateViewMatrix(mat4 viewMatrix);
	void updateProjMatrix(float width, float height);
	void addModel(Model* m);
	mat4 getProjMatrix();
};

#endif // RENDERER_H