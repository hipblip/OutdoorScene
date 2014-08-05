
#include "EventManager.h"
#include "Renderer.h"

int main() {
	EventManager::setUp();
	Renderer::setUp();

	do { // Main Loop
		EventManager::update();

		Renderer::beginFrame();


		Renderer::endFrame();

	}while(EventManager::exitLoop() == false);

	EventManager::terminateProgram();
	return 0;
}


