#include "Ground.h"

/* Constructor which loads the model into the OBJLoader object.  The filepath is the location of the model file
 * which is then passed in the constructor for the model loader.
 */
Ground::Ground() {
	string filepath = "Models/wolf/Wolf.obj";
	model = new OBJLoader(&filepath[0]);
}

Ground::~Ground() {
}

// The ground shouldn't need any animations
void Ground::update(float dt) {
}

/* Draws the model loaded from the file specified in the constructor
 * @param void
 * @return void
 */
void Ground::draw() {
	model->draw(Renderer::getShader());
}