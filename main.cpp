#include "EventManager.h"
#include "Renderer.h"
#include "Ground.h"
#include "World.h"
#include <string>

int main() {
	EventManager::setUp();
	Renderer::setUp();

	World world;

	GLuint worldMatrixID = glGetUniformLocation(Renderer::getShader(), "WorldTransform");
	GLuint viewMatrixID = glGetUniformLocation(Renderer::getShader(), "ViewTransform");
	GLuint projMatrixID = glGetUniformLocation(Renderer::getShader(), "ProjectionTransform");
	
	Ground *test = new Ground();
	world.addModel(test);

	do { // Main Loop
		EventManager::update();

		Renderer::beginFrame();

		world.draw();
		
		Renderer::endFrame();

	}while(EventManager::exitLoop() == false);

	EventManager::terminateProgram();
	Renderer::terminateProgram();
	return 0;
}
