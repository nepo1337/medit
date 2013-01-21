#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#define GLEW_STATIC
#include <glew.h>

#include "glm/glm.hpp"

using namespace glm;

class BoundingBox
{
private:
	vec3 bboxPos;
	vec3 bboxSide;
	GLuint vaoh,vboh;
public:
	BoundingBox();
	~BoundingBox();
	void setBboxPos(vec3 p);
	void setBboxSide(vec3 s);
	vec3 getBboxPos();
	vec3 getBboxSide();
	
	void setVaoh(GLuint vaoh);
	GLuint getVaoh();

};

#endif // BOUNDINGBOX_H
