#include "StaticCamera.h"


using namespace glm;

StaticCamera::StaticCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector) {
	mPosition = position;
	mLookAtPoint = lookAtPoint;
	mUpVector = upVector;
}

StaticCamera::~StaticCamera()
{
}

void StaticCamera::Update(float dt)
{

}

glm::mat4 StaticCamera::GetViewMatrix() const
{
	return glm::lookAt(		mPosition,		// Camera position
							mLookAtPoint,	// Look towards this point
							mUpVector		// Up vector
						);
}

glm::mat4 StaticCamera::GetViewProjectionMatrix() const
{
	// @TODO 1 - Calculate View Projection Matrix
	//glm::mat4 viewProjection(1.0f); // identity, you need to change this

	// Solution
	return GetProjectionMatrix() * GetViewMatrix();

//	return viewProjection;
}

glm::mat4 StaticCamera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}
