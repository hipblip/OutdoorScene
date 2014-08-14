//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//
//Modifications to this document performed by David Paparo ID# 6664334

#include "FirstPersonCamera.h"
#include "EventManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>



using namespace glm;

FirstPersonCamera::FirstPersonCamera(glm::vec3 position) :  Camera(), mPosition(position), mLookAt(0.0f, 0.0f, -1.0f), mHorizontalAngle(90.0f), mVerticalAngle(0.0f), mSpeed(5.0f), mAngularSpeed(25.0f), jumpFlag(false), jumpTime(0.0f)
{
}

FirstPersonCamera::~FirstPersonCamera()
{
}

//All functions of the camera are held here
void FirstPersonCamera::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();


	// Mouse motion to get the variation in angle
	mHorizontalAngle -= EventManager::GetMouseMotionX() * mAngularSpeed  * dt;
	mVerticalAngle   -= EventManager::GetMouseMotionY() * mAngularSpeed  * dt;

	// Clamp vertical angle to [-85, 85] degrees
	mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
	if (mHorizontalAngle > 360)
	{
		mHorizontalAngle -= 360;
	}
	else if (mHorizontalAngle < -360)
	{
		mHorizontalAngle += 360;
	}

	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);

	mLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
	
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);


	


	//Adjustement based on your lookAt pitch, to simulate the inability to move quickly as you star higher up
	//20 degree angle will begin to cut walk speed, 45 and higher will reduce speed even further
	pitchRadians = asin(mLookAt.y);
	if(pitchRadians >= 0.349)
	{
		angleReductionSpeed = 0.75f;
	}
	if(pitchRadians >= 0.785)
	{
		angleReductionSpeed = 0.5f;
	}
	if(pitchRadians <= 0.349)
	{
		angleReductionSpeed = 1.0f;
	}

	//RunSpeed, to allow an increase should it be needed, runfunction is disabled should angle exceed
	if(glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && pitchRadians <= 0.785)
	{
		runSpeed = 2.5f;
	}
	else{
		runSpeed = 1.0f;
	}

	//Diagonal limitation, to ensure that speed isn't increased when using diagonal motion
	if(
		(glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS && glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
		|| (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS && glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
		|| (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS && glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
		|| (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS && glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS))
	{
		doubleKeyAdjust = 0.7071f;
	}
	else{
		doubleKeyAdjust = 1.0f;
	}

	// A S D W for motion along the XZ Plane, Y cordinate will be handled by Jump feature
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
	{
		mPosition += normalize(vec3(mLookAt.x, 0, mLookAt.z)) * dt * mSpeed * runSpeed * angleReductionSpeed * doubleKeyAdjust;
	}

	//Run not available when backing up
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
	{
		mPosition -= normalize(vec3(mLookAt.x, 0, mLookAt.z)) * dt * mSpeed * angleReductionSpeed * doubleKeyAdjust;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
	{
		mPosition += normalize(vec3(sideVector.x, 0, sideVector.z)) * dt * mSpeed * runSpeed * angleReductionSpeed * doubleKeyAdjust;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
	{
		mPosition -= normalize(vec3(sideVector.x, 0, sideVector.z)) * dt * mSpeed * runSpeed * angleReductionSpeed * doubleKeyAdjust;
	}

	//Jump feature, checks to ensure the camera is on the ground before being able to start the jump, then increases y value over time, hitting a peak then decreasing back to the floor value
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE ) == GLFW_PRESS && jumpFlag == false)
	{
		
		jumpFlag = true;
	}
	//Y increase taken through trial and error, what was felt to be best fit to not be too straineous on the eyes
	if (jumpFlag == true){
		jumpTime += dt;
		mPosition += (vec3(0, 0.7f, 0) * jumpTime) - ((vec3(0, 4.9f, 0) * jumpTime * jumpTime * 0.5f));
	}
	//To be changed to relfect position on floor
	if(mPosition.y <= 1.0f){
		jumpTime = 0.0f;
		jumpFlag = false;
		mPosition.y = 1.0f;
	}


}

//Taken From Assignment 1
glm::mat4 FirstPersonCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}

//Taken From Assignment 1
glm::vec3 FirstPersonCamera::GetPosition() const
{
	return mPosition;
}