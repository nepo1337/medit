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
#include "RadiusRing.h"
#include <fstream>



using namespace std;
using namespace glm;

class LightHandler
{
private:
	GLSLProgram lightShader,bBoxShader;
	vector<Light> lights;
	MeshHandler meshes;
	int spotLightMeshPos;
	int pointLightMeshPos;
	int ambientLightMeshPos;
	RadiusRing radRing;
public:
	LightHandler();
	~LightHandler();
	void init();
	void addLight(Light l);
	void drawLights(mat4 projectionMatrix, mat4 viewMatrix);
	void drawLight(mat4 projectionMatrix, mat4 viewMatrix,Light light);
	void removeLightsBoundToModels(vector<Model> models);
	int selectLight(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix);
	void removeSelectedLights();
	void deselectAllLights();
	void free();
	Light getSelectedLight();
	int getSelectedLightIndex();
	void save(string path, string filename);
	void assignLightAnotherLight(Light l);
	void clear();
	int getNrOfLights();
};
#endif // LIGHTHANDLER_H
