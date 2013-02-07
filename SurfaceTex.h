#ifndef SURFACETEX_H
#define SURFACETEX_H

#define GLEW_STATIC
#include <glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "SOIL/SOIL.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

class SurfaceTex
{
private:
	GLuint texHandle;
	GLuint vaoh;
	GLuint vbohs[4];
	vector<mat4> modelMatrices;
	vector<vec3> positions;
	vector<float> rotations;
	vector<bool> showBbox;
	vector<float> scales;
	string name;
public:
	SurfaceTex();
	~SurfaceTex();
	void init(string path, string texName);
	GLuint getTexHandle();
	GLuint getVaoH();
	vector<vec3>* getPositions();
	vector<float>* getRotations();
	vector<bool>* getDrawBbox();
	vector<float>* getScales();
	void addSurface(float rot, vec3 pos,float scale);
	string getName();
	void select(int index);
	void deSelect(int index);
	bool isSelected(int index);
	void remove(int index);
	vector<mat4>* getModelMatrices();
};

#endif // SURFACETEX_H
