#ifndef PARTICLEHANDLER_H
#define PARTICLEHANDLER_H

#define GLEW_STATIC
#include <glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>
#include "GLSLProgram.h"
#include "MeshHandler.h"
#include "MeshInfo.h"
#include "Particle.h"
#include <vector>
#include <fstream>

using namespace std;
using namespace glm;

class ParticleHandler
{
private:
	GLSLProgram particleShader,bBoxShader;
	MeshHandler meshes;
	vector<Particle> particleModels;
public:
	ParticleHandler();
	~ParticleHandler();
	void init();
	void addParticleModel(Particle p);
	void drawParticleModels(mat4 projectionMatrix, mat4 viewMatrix);
	void drawParticleModel(mat4 projectionMatrix, mat4 viewMatrix,Particle p);
	void removeSelectedParticles();
	int selectParticles(float normalizedX, float normalizedY,vec3 pos, mat4 projMatrix,mat4 viewMatrix);
	void unselectAllParticleModels();
	Particle getSelectedParticle();
	void assignParticleNewParticle(Particle p);
	void save(string path, string filename);
	void clear();
	int getNrOfParticleSystems();
};

#endif // PARTICLEHANDLER_H
