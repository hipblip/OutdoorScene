#pragma once

#include "Model.h"

#include <gl\GL.h>
#include "Particle.h"
#include "Camera.h"
#include <glm\gtx\norm.hpp>
#include <glm\glm.hpp>


//using namespace glm;

class RainEffect{
public:
	RainEffect(){}
	RainEffect(Camera* cam);
	~RainEffect();

	void Update(float dt, bool wind);
	void Draw();

	void SortParticles();
	int  FindUnusedParticle();

	void SetCamera(Camera* cam);

	Particle EmitRainParticle(Particle p);
	Particle UpdateRainParticle(Particle p, float dt, bool wind);

private:

	Camera*					mCamera;
	unsigned int			mMaxParticles;
	unsigned int			mLastUsedParticle;
	std::vector<Particle>	mParticlesContainer;

	GLfloat*				mPositionSizeData;
	GLubyte*				mColorData;
	
	unsigned int			mVertexArrayID;
	GLuint					mVertexBuffer;
	GLuint					mPositionBuffer;
	GLuint					mColorBuffer;
	unsigned int			ParticlesCount;
};
