#ifndef SURFACECIRCLE_H
#define SURFACECIRCLE_H

#define GLEW_STATIC
#include <glew.h>
#include <iostream>

#include "GLSLProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include "SOIL/SOIL.h"

using namespace std;
using namespace glm;

class SurfaceCircle
{
private:
	vec3 position;
	float scale;
	GLSLProgram shader;
	GLuint vaoh;
	GLuint vbohs[2];
	GLuint texHandle;
public:
	SurfaceCircle();
	~SurfaceCircle();
	void init();
	void setPosition(vec3 pos);
	vec3 getPosition();
	void setScale(float s);
	float getscale();
	void draw(mat4 projectionMatrix, mat4 viewMatrix);

};

#endif // SURFACECIRCLE_H
