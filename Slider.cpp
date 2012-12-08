#include "Slider.h"

Slider::Slider(vec3 pos)
{
	this->pos=pos;
	this->maxXVal=0;
	this->maxYVal=0;
	this->minXVal=0;
	this->minYVal=0;
}

Slider::Slider()
{
	this->pos=vec3(0.0f);
	this->maxXVal=0;
	this->maxYVal=0;
	this->minXVal=0;
	this->minYVal=0;
}

Slider::~Slider()
{
}


bool Slider::setPositionX(float x)
{
	bool valid=false;
	if(x>=this->minXVal&&x<=this->maxXVal)
	{
		this->pos.x=x;
		valid=true;
	}
	return valid;
}

bool Slider::setPositionY(float y)
{
	bool valid=false;
	if(y>=this->minYVal&&y<=this->minYVal)
	{
		this->pos.y=y;
		valid=true;
	}
	return valid;
}

vec3 Slider::getPosition()
{
	return this->pos;
}
float Slider::getSliderValueX()
{
	return 1-(this->maxXVal-this->pos.x)/(this->maxXVal-this->minXVal);
}

float Slider::getSliderValueY()
{
	return 1-(this->maxYVal-this->pos.y)/(this->maxYVal-this->minYVal);
}
void Slider::setMaxPos(float x, float y)
{
	this->maxXVal=x;
	this->maxYVal=y;
}
void Slider::setMinPos(float x, float y)
{
	this->minXVal=x;
	this->minYVal=y;
}

bool Slider::isInsideSliderSpace(float x, float y)
{
	return (x<=this->maxXVal&&x>=this->minXVal&&y<=this->maxYVal&&y>=this->minYVal);
}