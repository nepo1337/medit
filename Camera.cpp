#include "Camera.h"

Camera::Camera()
{
	this->eye=vec3(0.0f,1.0f,0.0f);
	this->lookat=vec3(0.0f,0.0f,-1.0f);
	this->up=vec3(0.0f,1.0f,0.0f);
	this->viewMatrix=mat4(1.0f);
	this->rotx=this->roty=this->rotz=0;
	this->calcViewMatrix();
}

Camera::~Camera()
{
}
void Camera::calcViewMatrix()
{
	this->viewMatrix=glm::lookAt(eye,lookat,up);
}
mat4 Camera::getViewMatrix()
{
	return this->viewMatrix;
}

vec3 Camera::getPos()
{
	return this->eye;
}
void Camera::moveForeward(float a)
{
	this->eye.z-=a;
	this->lookat.z-=a;
	this->calcViewMatrix();
}
void Camera::moveBackward(float a)
{
	this->eye.z+=a;
	this->lookat.z+=a;
	this->calcViewMatrix();
}
void Camera::strafeLeft(float a)
{
	this->eye.x-=a;
	this->lookat.x-=a;
	this->calcViewMatrix();
}
void Camera::strafeRight(float a)
{
	this->eye.x+=a;
	this->lookat.x+=a;
	this->calcViewMatrix();
}
void Camera::zoomIn(float a)
{
	vec3 asd= normalize((this->eye-this->lookat));
	if(this->eye.y-asd.y*a>1.0f)
		this->eye-=asd*a;
	this->calcViewMatrix();
}
void Camera::zoomOut(float a)
{
	vec3 asd= normalize((this->eye-this->lookat));
	this->eye+=asd*a;
	this->calcViewMatrix();
}