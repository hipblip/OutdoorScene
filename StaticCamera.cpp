//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//
//Modifications to this document performed by David Paparo ID# 6664334

#include "StaticCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "Ground.h"

//using namespace glm;


StaticCamera::StaticCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector, Ground* inGround)
	: Camera(inGround), mPosition(position), mLookAtPoint(0,0,0), mUpVector(upVector), movement(0.0f), modifier(1.0f), radius(30.0f)
{
}

StaticCamera::~StaticCamera()
{
}

//Moves Camera on a set circular path through the scene while maintaning a look at of (0,0,0)
void StaticCamera::Update(float dt)
{
	//CCW direction movement
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
	{
		camFlag = true;
	}

	//CC direction movement
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
	{
		camFlag = false;
	}

	//Normal Movement Movement
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
	{
		modifier = 1.0f;
	}

	//Slow Movement Movement
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
	{
		modifier = 0.5f;
	}


	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE ) == GLFW_RELEASE){
		if(camFlag){
			movement += dt;
		}
		else{
			movement -= dt;
		}
		mPosition.x = sin(50.0f * movement * modifier * dt)* radius;
		mPosition.z = cos(50.0f * movement * modifier * dt)* radius;
		groundHeight = ((terrain->getHeight(mPosition.x + 1, mPosition.z)) + (terrain->getHeight(mPosition.x - 1, mPosition.z)) + (terrain->getHeight(mPosition.x, mPosition.z + 1)) + (terrain->getHeight(mPosition.x, mPosition.z - 1)))/4;
		mPosition.y = groundHeight + 3;
	}
	else{//Pause camera
		mPosition.x = mPosition.x;
		mPosition.z = mPosition.z;
	}
	EventManager::EnableMouseCursor();
}

glm::mat4 StaticCamera::GetViewMatrix() const
{
	return glm::lookAt(		mPosition,		// Camera position
							mLookAtPoint,	// Look towards this point
							mUpVector		// Up vector
						);
}

//Returns lookAt vector so that the particle system to allign the billboards
glm::vec3 StaticCamera::GetLookAt() const{
	return mLookAtPoint;
}

//Returns position vector so that the particle system to allign the billboards
glm::vec3 StaticCamera::GetPosition() const{
	return mPosition;
}