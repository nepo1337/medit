#ifndef PARTICLE_H
#define PARTICLE_H
#include "model.h"

namespace ParticleType
{
  enum ParticleTypes
  {
    EMIT,FIRE,FLOW,GLOWRING,SMOKE
  };
};


class Particle : public Model
{
private:
	ParticleType::ParticleTypes particleType;
	vec3 color;
	float contrast;
public:
	Particle();
	~Particle();
	ParticleType::ParticleTypes getParticleType();
	void setParticleType(ParticleType::ParticleTypes type);
	vec3 getColor();
	void setColor(vec3 c);
	void setContrast(float f);
	float getContrast();
};

#endif // PARTICLE_H
