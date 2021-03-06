#include "Camera.h"

#define PI 3.14159265

Camera::Camera()
{
	this->eye=vec3(2.0f,2.0f,0.0f);
	this->lookat=vec3(0.0f,0.0f,0.0f);
	this->up=vec3(0.0f,1.0f,0.0f);
	this->viewMatrix=mat4(1.0f);
	this->rotx=0;
	this->roty=0;
	this->rotz=80;
	this->distance=5;
	
	this->eye.x=sin(rotz*PI/180)*sin(roty*PI/180)*this->distance+this->lookat.x;
	this->eye.y=cos(rotz*PI/180)*this->distance+this->lookat.y;
	this->eye.z=sin(rotz*PI/180)*cos(roty*PI/180)*this->distance+this->lookat.z;
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

//*0.1 represents the speed, as the direction gets normalized, then scaled
void Camera::moveForeward(float a)
{
	vec3 dir= ((this->eye-this->lookat));
	vec3 dirMoved=normalize(vec3(dir.x,0,dir.z))*-a;
	this->eye+=dirMoved;
	this->lookat+=dirMoved;
	this->calcViewMatrix();
}
void Camera::moveBackward(float a)
{
	vec3 dir= ((this->eye-this->lookat));
	vec3 dirMoved=normalize(vec3(dir.x,0,dir.z))*a;
	this->eye+=dirMoved;
	this->lookat+=dirMoved;
	this->calcViewMatrix();
}
void Camera::strafeLeft(float a)
{
	vec3 dir= ((this->eye-this->lookat));
	vec3 dirMoved=normalize(vec3(dir.x,0,dir.z))*a;
	vec3 crossed = cross(dirMoved,vec3(0.0,1.0,0.0));
	this->eye+=crossed;
	this->lookat+=crossed;
	this->calcViewMatrix();
}
void Camera::strafeRight(float a)
{
	vec3 dir= ((this->eye-this->lookat));
	vec3 dirMoved=normalize(vec3(dir.x,0,dir.z))*-a;
	vec3 crossed = cross(dirMoved,vec3(0.0,1.0,0.0));
	this->eye+=crossed;
	this->lookat+=crossed;
	this->calcViewMatrix();
}
void Camera::zoomIn(float a)
{
	if(length(this->eye-this->lookat)-a>0.5)
	{
		this->distance-=a;
		//vec3 dir= normalize((this->eye-this->lookat));
		//if(this->eye.y-dir.y*a>0.4f)
			//this->eye+=this->distance;
			
		this->eye.x=sin(rotz*PI/180)*sin(roty*PI/180)*this->distance+this->lookat.x;
		this->eye.y=cos(rotz*PI/180)*this->distance+this->lookat.y;
		this->eye.z=sin(rotz*PI/180)*cos(roty*PI/180)*this->distance+this->lookat.z;
		
		this->calcViewMatrix();
	}
}
void Camera::zoomOut(float a)
{
	if(length(this->eye-this->lookat)+a>0.5)
	{
		this->distance+=a;
		//vec3 dir= normalize((this->eye-this->lookat));
		//this->eye+=this->distance;
		
		this->eye.x=sin(rotz*PI/180)*sin(roty*PI/180)*this->distance+this->lookat.x;
		this->eye.y=cos(rotz*PI/180)*this->distance+this->lookat.y;
		this->eye.z=sin(rotz*PI/180)*cos(roty*PI/180)*this->distance+this->lookat.z;
		
		this->calcViewMatrix();
	}
}
void Camera::rotateLeft(float deg)
{
	this->roty+=deg;
	this->eye.x=sin(rotz*PI/180)*sin(roty*PI/180)*this->distance+this->lookat.x;
	this->eye.y=cos(rotz*PI/180)*this->distance+this->lookat.y;
	this->eye.z=sin(rotz*PI/180)*cos(roty*PI/180)*this->distance+this->lookat.z;
	this->calcViewMatrix();
	/*
	//calculates a vector for rotating around the lookat point
	vec3 dir= ((this->eye-this->lookat));
	mat4 rot(1.0f);
	rot*=scale(dir);
	rot*=rotate(deg*-0.2f,vec3(0,1,0));

	vec4 t(1.0);
	t=t*rot;
	vec3 f=vec3(t.x,t.y,t.z);
	
	this->eye.x=(this->lookat.x+f.x);
	this->eye.y=(this->lookat.y+f.y);
	this->eye.z=(this->lookat.z+f.z);

	this->calcViewMatrix();
	 */
}

void Camera::rotateUp(float deg)
{
	this->rotz+=deg;
	//a few if to avoid angle 0
	if(rotz==0)
		rotz=1;
	if(rotz==180)
		rotz=181;
	if(rotz>=360)
		rotz=1;
	if(rotz<=-180)
		rotz=181;
	this->eye.x=sin(rotz*PI/180)*sin(roty*PI/180)*this->distance+this->lookat.x;
	this->eye.y=cos(rotz*PI/180)*this->distance+this->lookat.y;
	this->eye.z=sin(rotz*PI/180)*cos(roty*PI/180)*this->distance+this->lookat.z;
	this->calcViewMatrix();
	/*
	this->eye.y+=deg*-0.04;

	this->calcViewMatrix();
	 */
}