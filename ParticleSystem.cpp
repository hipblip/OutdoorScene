#pragma once

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
	mpCamera = NULL;
	mpEmitter = NULL;
	mLastUsedParticle = 0;
	mParticles = std::vector<Particle>();
	InitializeBuffers();
}

ParticleSystem::~ParticleSystem(){
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void ParticleSystem::SetCamera(Camera* camera){
	mpCamera = camera;
}

void ParticleSystem::SetEmitter(ParticleEmitter* emitter){
	mpEmitter = emitter;
}

void ParticleSystem::InitializeBuffers(){

	unsigned int numParticles = mpEmitter->GetNumParticles();

	for(int i=0; i < numParticles; ++i){
		Particle p = mpEmitter->InitializeParticle();
		mParticles.push_back(p);
	}

	mParticlePositionSizeData = new GLfloat[mParticles.size() * 4];
	mPaticleColorData		  = new GLubyte[mParticles.size() * 4];

	GLfloat mVertexBufferData[]={ -0.5f, -0.5f, 0.0f, 
								   0.5f, -0.5f, 0.0f,
							      -0.5f,  0.5f, 0.0f, 
								   0.5f,  0.5f, 0.0f };

	mVertexArrayID = 0;

	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);

	glGenBuffers(1, &mBillboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBillboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexBufferData), mVertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &mParticlePositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, numParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &mParticleColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, numParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void ParticleSystem::SortParticles(){
	std::sort(mParticleBuffer.begin(), mParticleBuffer.end());
}

void ParticleSystem::Update(float dt){
	int newParticles = (int)(dt*mpEmitter->GetNumberNewParticles());
	if(dt > (int)(0.016f)){
		newParticles = (int)(0.016f*mpEmitter->GetNumberNewParticles());
	}

	for(int i=0; i<newParticles; ++i){
		int particleIndex = FindUnusedParticleIndex();
		mpEmitter->EmitParticle(mParticles[particleIndex]);
	}

	int mParticleCount = 0;
	for(int i=0; i<mParticles.size(); ++i){

		Particle& p = mParticles[i];
		
		if(p.age > 0.0f){
			p.age -= dt;
			if(p.age > 0.0f){
				p = mpEmitter->UpdateParticle(p);
				
				mParticlePositionSizeData[4*mParticleCount+0] = p.position.x;
				mParticlePositionSizeData[4*mParticleCount+1] = p.position.y;
				mParticlePositionSizeData[4*mParticleCount+2] = p.position.z;
				mParticlePositionSizeData[4*mParticleCount+3] = p.size;

				mPaticleColorData		 [4*mParticleCount+0] = p.color.x;
				mPaticleColorData		 [4*mParticleCount+1] = p.color.y;
				mPaticleColorData		 [4*mParticleCount+2] = p.color.z;
				mPaticleColorData		 [4*mParticleCount+3] = p.color.w;
			
			}else{
				p.cameraDistance = -1.0f;
			}

			++mParticleCount;
		}
	}

	SortParticles();
}

void ParticleSystem::Draw(){
	
	glBindBuffer(GL_ARRAY_BUFFER, mParticlePositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mParticles.size() * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * sizeof(GLfloat) * 4, mParticlePositionSizeData);

	glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mParticles.size() * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * sizeof(GLubyte) * 4, mPaticleColorData);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);*/

	/*glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
	*/

	// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mBillboardVertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);


		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mParticlePositionBuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mParticleColorBuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad  

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mParticleCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
}

int ParticleSystem::FindUnusedParticleIndex(){

	for(int i=mLastUsedParticle; i<mParticles.size(); i++){
		if (mParticles[i].age < 0){
			mLastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<mLastUsedParticle; i++){
		if (mParticles[i].age < 0){
			mLastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}