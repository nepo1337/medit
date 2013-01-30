#ifndef PATH_H
#define PATH_H
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#define GLEW_STATIC
#include <glew.h>
#include <iostream>

using namespace glm;
using namespace std;

class Path
{
private:
	vector<vec3> flagPositions;
	vec3 color;
	//handles for drawing lines
	GLuint vaoh;
	GLuint vboh;
	bool selected;
public:
	Path();
	~Path();
	void addFlag(vec3 pos);
	vec3 getFlagPosition(int i);
	int getNrOfFlags();
	void setColor(vec3 c);
	vec3 getColor();
	GLuint getVaohToLines();
	void freeGFX();
	void select();
	void unselect();
	bool isSelected();
};

#endif // PATH_H
