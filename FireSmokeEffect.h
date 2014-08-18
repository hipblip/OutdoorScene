#pragma once

#include "Model.h"
#include <gl\GL.h>
#include "Particle.h"
#include "Camera.h"
#include <glm\gtx\norm.hpp>
#include <glm\glm.hpp>


using namespace glm;

class FireSmokeEffect{
public:
	FireSmokeEffect(){}
	FireSmokeEffect(Camera* cam);
	~FireSmokeEffect();

	void Update(float dt, bool wind);
	void Draw();

	void SetCamera(Camera* cam);

	void SortParticles();
	int  FindUnusedParticle();

	Particle EmitFireSmokeParticle(Particle p);
	Particle UpdateFireSmokeParticle(Particle p, float dt, bool wind);

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
