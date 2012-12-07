#ifndef SPRITE_H
#define SPRITE_H

#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <glew.h>
#include <iostream>

using namespace std;
using namespace glm;

class Sprite
{
private:
	GLuint vaoHandle;
	GLuint texHandle;
	GLuint vboHandle[2];
	mat4 modelMatrix;
	vec3 position;
public:
	Sprite();
	~Sprite();
	bool init(vec3 pos, float width, float height, string textureName);
	bool init(vec3 pos, float width, float height);
	GLuint getVaoHandle();
	GLuint getTextureHandle();
	vec3 getPosition();
	void setPosition(vec3 pos);
	mat4 getModelMatrix();
};

#endif // SPRITE_H
