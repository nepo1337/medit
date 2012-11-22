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

using namespace std;
using namespace glm;
class Renderer
{
private:
	GLSLProgram modelShader;
	mat4 viewMatrix,projMatrix;
	bool debug;
	vector<Model*> models;
public:
	Renderer();
	~Renderer();
	void draw();
	void updateCamera(vec3 pos);
	void updateProjMatrix(float width, float height);
	void addModel(Model* m);

};

#endif // RENDERER_H