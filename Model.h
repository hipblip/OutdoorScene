#pragma once

#include <GLM/glm.hpp>
#include "OBJLoader.h"

class Model {
public:
	
	Model(Model *parent);
	Model();
	virtual ~Model();

	virtual void update(float dt) = 0;
	virtual void draw() = 0;

	void setPosition(glm::vec3 pos) { mPosition = pos; };
	void setScaling(glm::vec3 scale) { mScaling = scale; };
	void setRotationAxis(glm::vec3 axis) { mRotationAxis = axis; };
	void setRotationAngle(float angle) {mRotationAngle = angle; };

	glm::mat4 getWorldMatrix();
	
private:
	glm::vec3 mPosition;
	glm::vec3 mScaling;
	glm::vec3 mRotationAxis;
	float     mRotationAngle;

	Model* parent;
	//OBJLoader* importedModel; // Only used if the model is imported
};