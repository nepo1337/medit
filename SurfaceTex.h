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
	vector<float> rotations;
	vector<vec3> positions;
	vector<bool> showBbox;
	string name;
public:
	SurfaceTex();
	~SurfaceTex();
	void init(string path, string texName);
	GLuint getTexHandle();
	GLuint getVaoH();
	vector<float>* getRotations();
	vector<vec3>* getPositions();
	vector<bool>* getDrawBbox();
	void addSurface(float rot, vec3 pos);
	string getName();
	void select(int index);
	void deSelect(int index);
	bool isSelected(int index);
	void remove(int index);

};

#endif // SURFACETEX_H
