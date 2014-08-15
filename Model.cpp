

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "Model.h"

/* Assign default values to the members and initializes the parent member.  This constructor is only called when the model uses a hierarchy.
 * @param the model's parent
 * @return void
 */
Model::Model(Model* parent) : mPosition(0.f, 0.f, 0.f), mScaling(1.f, 1.f, 1.f), mRotationAxis(0.f, 1.f, 0.f), mRotationAngle(0.f) {
	Model::parent = parent;
}

/* The default constructor for the model which initializes all the members to the default values.
 * @param void
 * @return void
 */
Model::Model() : mPosition(0.f, 0.f, 0.f), mScaling(1.f, 1.f, 1.f), mRotationAxis(0.f, 1.f, 0.f), mRotationAngle(0.f) {
	parent = NULL;
}

Model::~Model() {
}

void Model::update(float dt) {
}

void Model::draw() {
}

/* This function creates the scaling, translation, and rotation matrices from the members and creates the worldMatrix.  This function is called 
 * in the world class when everything is being drawn.
 * @param void
 * @return the world matrix
 */
glm::mat4 Model::getWorldMatrix() {
	glm::mat4 scaling = glm::scale(glm::mat4(1.f), mScaling);
	glm::mat4 position = glm::translate(glm::mat4(1.f), mPosition);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.f), mRotationAngle, mRotationAxis);

	glm::mat4 worldMatrix = position * rotation * scaling;

	// If the model has a parent, concatenate the two world matrices
	if (parent) {
		worldMatrix = parent->getWorldMatrix() * worldMatrix;
	}

	return worldMatrix;
}