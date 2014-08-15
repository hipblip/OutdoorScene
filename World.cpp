#include "World.h"
#include "Ground.h"
#include <vector>
#include <iostream>

std::vector<Model*> models;

/* Initialize all the models here in the constructor
 */
World::World() {

}

World::~World() {
	for (std::vector<Model*>::iterator it = models.begin(); it < models.end(); ++it) {
		//delete *it;
	}

	models.clear();
}

// float dt is the frame time
void World::update(float dt) {

	//Update the models
	for (std::vector<Model*>::iterator it = models.begin(); it < models.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void World::draw() {
	// Get the shader
	//Renderer::setShader(1);
	//glUseProgram(Renderer::getShader());

	//GLuint WorldMatrixID;
	//GLuint ViewMatrixID;
	//GLuint ProjMatrixID;

	//// TODO: replace dummy values for the view and projection matrices with the actual values from the camera
	//glm::mat4 view = glm::lookAt(glm::vec3(4,3,3), // cam Position
	//							 glm::vec3(0,0,0), // look at
	//							 glm::vec3(0,1,0));// up vector
	//glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	// Get a handle on the transform variables of the shader
	//if (Renderer::getShaderVectorPosition() == 0) { // Using the model import shader		
	//	ViewMatrixID = glGetUniformLocation(Renderer::getShader(), "ViewTransform");
	//	ProjMatrixID = glGetUniformLocation(Renderer::getShader(), "ProjectonTransform");
	//	WorldMatrixID = glGetUniformLocation(Renderer::getShader(), "WorldTransform");

	

	//} else if (Renderer::getShaderVectorPosition() == 1) { // Using the Solid color shader
	//	GLuint VPmatrixID = glGetUniformLocation(Renderer::getShader(), "ViewProjectonTransform");
	//	//WorldMatrixID = glGetUniformLocation(Renderer::getShader(), "WorldTransform");
	//	glm::mat4 VP = projection * view;
	//	glUniformMatrix4fv(VPmatrixID, 1, GL_FALSE, &VP[0][0]);
	//}

	// Loop throught he models, get its transform, assign them to their respective variables in the shader, and draw the model
	for (std::vector<Model*>::iterator it = models.begin(); it < models.end(); ++it)  {

		//glm::mat4 worldMatrix = (*it)->getWorldMatrix();

		//if (Renderer::getShaderVectorPosition() == 0) {
		//	glm::mat4 worldMatrix = (*it)->getWorldMatrix();
		//	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);
		//	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &projection[0][0]);
		//	
		//}

		//glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &worldMatrix[0][0]);	

		(*it)->draw();
	}
}

void World::addModel(Model* toAdd) {
	models.push_back(toAdd);
}