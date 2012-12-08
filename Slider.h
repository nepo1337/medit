#ifndef SLIDER_H
#define SLIDER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform2.hpp"
#include <glew.h>
#include <iostream>

using namespace std;
using namespace glm;

//a small class to control the sliders
class Slider
{
private:
	vec3 pos;
	float maxXVal;
	float maxYVal;
	float minXVal;
	float minYVal;
public:
	Slider(vec3 pos);
	Slider();
	~Slider();
	bool setPositionX(float x);
	bool setPositionY(float y);
	vec3 getPosition();
	float getSliderValueX();
	float getSliderValueY();
	void setMaxPos(float x, float y);
	void setMinPos(float x, float y);
	bool isInsideSliderSpace(float x, float y);
};

#endif // SLIDER_H
