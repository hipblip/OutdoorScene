
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EventManager.h"
#include "Renderer.h"
#include "GrassGenerator.h"
#include "GrassModel.h"
#include "StaticCamera.h"

int main() {
	EventManager::setUp();
	Renderer::setUp();

	//corneliu: begin addition
	//I added this in order to make it show stuff. Feel free to move or replace this if you don't like it.
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	GLuint iShadersID = Renderer::LoadShaders( "../Source/Shaders/Phong.vertexshader", "../Source/Shaders/Phong.fragmentshader" );

	GLuint WorldMatrixID = glGetUniformLocation(iShadersID, "WorldTransform");
	GLuint ViewMatrixID = glGetUniformLocation(iShadersID, "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(iShadersID, "ProjectonTransform");

	// World Matrix - Identity - Draw Sphere at the origin
	glm::mat4 World(1.0f);
	
	// View matrix
	glm::mat4 View = glm::lookAt( glm::vec3(0,0,-5), glm::vec3(0,0,0), glm::vec3(0,1,0) );

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	StaticCamera sc( vec3(3.0f, 4.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) );

	//generating grass...
	GrassGenerator gg;
	vector<GrassModel> grass = gg.getGrass( 10000 );
	//corneliu: end of addition

	do { // Main Loop
		EventManager::update();		

		Renderer::beginFrame();

		//corneliu: begin addition
		//I added this in order to make it show stuff. Feel free to move or replace this if you don't like it.
		glUseProgram(iShadersID);
		GLuint VPMatrixLocation = glGetUniformLocation(iShadersID, "ViewProjectonTransform");
		glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &sc.GetViewProjectionMatrix()[0][0]);
		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &World[0][0]);
		glUniformMatrix4fv(ViewMatrixID,  1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(ProjMatrixID,  1, GL_FALSE, &Projection[0][0]);

		//this is the most beautiful grass in the world. Don't you dare to mow the lawn!
		for( vector<GrassModel>::iterator it = grass.begin(); it != grass.end(); ++it ) {
			it->Draw();
		}
		//corneliu: end of addition

		Renderer::endFrame();

	}while(EventManager::exitLoop() == false);

	EventManager::terminateProgram();
	return 0;
}
