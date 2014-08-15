#pragma once

#include "Particle.h"

class ParticleEmitter{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	virtual void EmitParticle(Particle particle);

	unsigned int GetNumParticles(){return numParticles;}
	unsigned int GetNumberNewParticles(){return newParticles;}

	Particle InitializeParticle();
	Particle UpdateParticle(Particle& p);

private:
	unsigned int	numParticles;
	unsigned int	newParticles;
};