#pragma once

#include <glm\glm.hpp>

struct Particle{

	glm::vec3 position; 
	glm::vec3 velocity;
	unsigned char r,g,b,a; // Color
	float size; 
	float angle;
	float weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

