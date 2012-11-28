#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <iostream>

using namespace std;
using namespace glm;

class Camera
{
private:
	mat4 viewMatrix;
	vec3 lookat, eye,up;
	void calcViewMatrix();
	float rotx,roty,rotz;
public:
	Camera();
	~Camera();
	mat4 getViewMatrix();
	vec3 getPos();
	void moveForeward(float a);
	void moveBackward(float a);
	void strafeLeft(float a);
	void strafeRight(float a);
	void zoomOut(float a);
	void zoomIn(float a);

};

#endif // CAMERA_H
