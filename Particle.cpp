#include "Particle.h"

Particle::Particle()
{
	this->color=vec3(1.0f);
	this->contrast=1.0f;
	this->type = ParticleType::FIRE;
}

Particle::~Particle()
{
}

void Particle::setParticleType(ParticleType::ParticleTypes type)
{
	this->particleType = type;
}

ParticleType::ParticleTypes Particle::getParticleType()
{
	return this->particleType;
}
void Particle::setColor(vec3 c)
{
	this->color=c;
}
void Particle::setContrast(float f)
{
	this->contrast=f;
}
float Particle::getContrast()
{
	return this->contrast;
}
vec3 Particle::getColor()
{
	return this->color;
}