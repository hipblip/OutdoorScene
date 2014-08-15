#pragma once

#include "Particle.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include <GL\glew.h>
#include <vector>
#include <algorithm>

 class ParticleSystem{
 
 public:
	 ParticleSystem();
	 ~ParticleSystem();
	 
	 void SetCamera(Camera* camera);
	 void SetEmitter(ParticleEmitter* emitter);

	 void InitializeBuffers();
	 void SortParticles();

	 void Update(float dt);
	 void EmitParticle(Particle particle);
	 void UpdateVertexBuffer();
	 void Draw();
	 int  FindUnusedParticleIndex();

 private:

	 Camera*					mpCamera;
	 ParticleEmitter*			mpEmitter;
	 unsigned int				mLastUsedParticle;
	 unsigned int				mParticleCount;
	 std::vector<Particle>		mParticles;
	 GLuint						mVertexArrayID;
	 GLfloat*					mParticlePositionSizeData;
	 GLubyte*					mPaticleColorData;
	 GLuint						mBillboardVertexBuffer;
	 GLuint						mParticlePositionBuffer;
	 GLuint						mParticleColorBuffer;
 };