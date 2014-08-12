#pragma once
#include <glm/gtc/matrix_transform.hpp>

class StaticCamera {

public:
	StaticCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector);
	~StaticCamera();

	void Update(float dt);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mLookAtPoint;
	glm::vec3 mUpVector;
};
