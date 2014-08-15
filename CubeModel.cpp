//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//

#include "CubeModel.h"
#include "Renderer.h"

// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

//using namespace glm;

CubeModel::CubeModel(glm::vec3 size, Model *mparent) : Model(mparent)
{
	
	// Create Vertex Buffer for all the verices of the Cube
	glm::vec3 halfSize = size * 0.5f;
	
	Vertex vertexBuffer[] = {  // position,                normal,                  color
								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) }, //left - red
								{ glm::vec3(-halfSize.x,-halfSize.y, halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y, halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },

								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y, halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y,-halfSize.z), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
								
								{ glm::vec3( halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) }, // far - blue
								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },

								{ glm::vec3( halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
								{ glm::vec3( halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f, 0.0f,-1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },

								{ glm::vec3( halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) }, // bottom - turquoise
								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) },
								{ glm::vec3( halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) },
																
								{ glm::vec3( halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) },
								{ glm::vec3(-halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) },
								{ glm::vec3(-halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f) },

								{ glm::vec3(-halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }, // near - green
								{ glm::vec3(-halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
								{ glm::vec3( halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },

								{ glm::vec3( halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
								{ glm::vec3( halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f) },

								{ glm::vec3( halfSize.x, halfSize.y, halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) }, // right - purple
								{ glm::vec3( halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) },
								{ glm::vec3( halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) },

								{ glm::vec3( halfSize.x,-halfSize.y,-halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) },
								{ glm::vec3( halfSize.x, halfSize.y, halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) },
								{ glm::vec3( halfSize.x,-halfSize.y, halfSize.z), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f) },

								{ glm::vec3( halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) }, // top - yellow
								{ glm::vec3( halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },

								{ glm::vec3( halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y,-halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
								{ glm::vec3(-halfSize.x, halfSize.y, halfSize.z), glm::vec3( 0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f) }
						};

	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

}

CubeModel::~CubeModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void CubeModel::update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
}

void CubeModel::draw()
{

	Renderer::setShader(1); // Solid color shader
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::getShader(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &getWorldMatrix()[0][0]);
	
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	0,				// attribute. No particular reason for 0, but must match the layout in the shader.
							3,				// size
							GL_FLOAT,		// type
							GL_FALSE,		// normalized?
							sizeof(Vertex), // stride
							(void*)0        // array buffer offset
						);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	1,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*)sizeof(glm::vec3)    // Normal is Offseted by vec3 (see class Vertex)
						);


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	2,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*) (2* sizeof(glm::vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
						);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}
