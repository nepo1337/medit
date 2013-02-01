#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

#define GLEW_STATIC
#include <glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>
#include <vector>
#include "Light.h"
#include "GLSLProgram.h"
#include "MeshHandler.h"
#include "MeshInfo.h"
#include "Model.h"


using namespace std;
using namespace glm;

class LightHandler
{
private:
	GLSLProgram lightShader,bBoxShader;
	vector<Light> pointLights;
	MeshHandler meshes;
	int spotLightMeshPos;
public:
	LightHandler();
	~LightHandler();
	void init();
	void addPointLight(vec3 pos, vec3 color,float radius,int id);
	void drawLights(mat4 projectionMatrix, mat4 viewMatrix);
	void removeLightsBoundToModels(vector<Model> models);
	int selectLight(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix);
	void removeSelectedLights();
};

#endif // LIGHTHANDLER_H
