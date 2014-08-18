#pragma once

#include "RainEffect.h"
#include "Renderer.h"
#include <GL/glew.h>

//using namespace glm;

RainEffect::RainEffect(Camera* cam){
	mCamera = cam;
	mMaxParticles = 10000;
	mParticlesContainer.resize(mMaxParticles,Particle());

	for(int i=0; i<mMaxParticles; i++){
		mParticlesContainer[i].life = -1.0f;
		mParticlesContainer[i].cameradistance = -1.0f;
	}

	/*mPositionSizeData		  = new GLfloat[mMaxParticles * 4];
	mColorData				  = new GLubyte[mMaxParticles * 4];
	// Create Vertex Buffer for all the verices of the Cube
	
	GLfloat vertexBufferData[] = { -0.5f, -0.5f, 0.0f,
									0.5f, -0.5f, 0.0f,
								   -0.5f,  0.5f, 0.0f,
									0.5f,  0.5f, 0.0f };

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	glGenBuffers(1, &mPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);*/

}

RainEffect::~RainEffect(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mColorBuffer);
	glDeleteBuffers(1, &mPositionBuffer);
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void RainEffect::Update(float dt){

	int newparticles = (int)(dt*400);
	if (newparticles > (int)(0.016f*400))
			newparticles = (int)(0.016f*400);
		
	for(int i=0; i<newparticles; i++){
		int particleIndex = FindUnusedParticle();
		mParticlesContainer[particleIndex] = EmitRainParticle(mParticlesContainer[particleIndex]);
	}

		glm::vec3 CameraPosition = mCamera->GetPosition();
		// Simulate all particles
		ParticlesCount = 0;

		mPositionSizeData		  = new GLfloat[mMaxParticles * 4];
		mColorData				  = new GLubyte[mMaxParticles * 4];

		for(int i=0; i<mMaxParticles; i++){

			Particle& p = mParticlesContainer[i]; // shortcut

			if(p.life > 0.0f){

				// Decrease life
				p.life -= dt;
				if (p.life > 0.0f){

					mParticlesContainer[i] = UpdateRainParticle(mParticlesContainer[i],dt);

					/*// Simulate simple physics : gravity only, no collisions
					p.velocity += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
					p.position += p.velocity * (float)dt;
					p.cameradistance = glm::length2( p.position - CameraPosition );*/

					// Fill the GPU buffer
					mPositionSizeData[4*ParticlesCount+0] = p.position.x;
					mPositionSizeData[4*ParticlesCount+1] = p.position.y;
					mPositionSizeData[4*ParticlesCount+2] = p.position.z;				   
					mPositionSizeData[4*ParticlesCount+3] = p.size;
												   
					mColorData[4*ParticlesCount+0] = p.r;
					mColorData[4*ParticlesCount+1] = p.g;
					mColorData[4*ParticlesCount+2] = p.b;
					mColorData[4*ParticlesCount+3] = p.a;

				}else{
					// Particles that just died will be put at the end of the buffer in SortParticles();
					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		SortParticles();
}

void RainEffect::Draw(){


	// Create Vertex Buffer for all the verices of the Cube
	
	GLfloat vertexBufferData[] = { -0.5f, -0.5f, 0.0f,
									0.5f, -0.5f, 0.0f,
								   -0.5f,  0.5f, 0.0f,
									0.5f,  0.5f, 0.0f };

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);
	glBindVertexArray(mVertexArrayID);


	// Vertex shader
	GLuint CameraRight_worldspace_ID  = glGetUniformLocation(Renderer::getShader(), "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID  = glGetUniformLocation(Renderer::getShader(), "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(Renderer::getShader(), "VP");

	glm::mat4 ViewMatrix = mCamera->GetViewMatrix();
	glm::mat4 ViewProjectionMatrix = mCamera->GetViewProjectionMatrix();


	// Same as the billboards tutorial
	glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);


	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);


	glGenBuffers(1, &mPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, mPositionSizeData);


	glGenBuffers(1, &mColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, mColorData);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	
	
	// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized? 
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	
		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1


		//glDrawArraysInstanced(GL_TRIANGLE_STRIP,0, 4, ParticlesCount);
		glDrawArraysInstanced(GL_POINTS, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

}

void RainEffect::SortParticles(){
	std::sort(mParticlesContainer.begin(), mParticlesContainer.end());
}

int RainEffect::FindUnusedParticle(){
	for(int i=mLastUsedParticle; i<mMaxParticles; i++){
		if (mParticlesContainer[i].life < 0){
			mLastUsedParticle = i;
			return i;
		}
	}

	for(int i=0; i<mLastUsedParticle; i++){
		if (mParticlesContainer[i].life < 0){
			mLastUsedParticle = i;
			return i;
		}
	}

	return 0; // if all particles are taken, override the first one
}

Particle RainEffect::EmitRainParticle(Particle p){
	p.life = 3.0f; // This particle will live 5 seconds.
	p.position = mCamera->GetPosition() + glm::vec3(0, 8, 0.0f);

	float spread = 10.5f;
			
	glm::vec3 maindir	= glm::vec3(0.0f, -1.0f, 0.0f);
			
	glm::vec3 randomdir = glm::vec3( (rand()%2000 - 1000.0f)/1000.0f,
									 (rand()%2000 - 1000.0f)/1000.0f,
									 (rand()%2000 - 1000.0f)/1000.0f );

	p.velocity = maindir + randomdir*spread;

	p.r = 255;
	p.g = 255;
	p.b = 255;
	p.a = 255;

	p.size = 0.05f;
	return p;
}

Particle RainEffect::UpdateRainParticle(Particle p, float dt){
	p.velocity += glm::vec3(0.0f,-18.81f, 0.0f) * (float)dt * 0.5f;
	p.position += p.velocity * (float)dt;
	p.cameradistance = glm::length2( p.position - mCamera->GetPosition());
	return p;
}

