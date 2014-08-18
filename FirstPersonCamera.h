//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
//
// Copyright (c) 2014 Concordia University. All rights reserved.
//
//Modifications to this document performed by David Paparo ID# 6664334

#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(glm::vec3 position, Ground* inGround);
	virtual ~FirstPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

	virtual glm::vec3 GetPosition() const;

	virtual glm::vec3 GetLookAt() const;

	virtual bool CheckPillarCollision() const;

	virtual bool CheckSphereCollision() const;

	virtual bool CheckSquareCollision() const;

private:
	//Assignment 1 original variables
	glm::vec3 mLookAt;
	glm::vec3 mPosition;	
	float mHorizontalAngle;		// horizontal angle
	float mVerticalAngle;		// vertical angle
	float mSpeed;				// World units per second
	float mAngularSpeed;		// Degrees per pixel

	//Added variables
	float runSpeed;				//Speed using shift or not
	float angleReductionSpeed;	//Speed reduction of looking too far upwards
	float pitchRadians;			//Pitch angle in radians
	float doubleKeyAdjust;		//Lowers speed when moving diagnoly to avoid faster movement through this form of movement
	bool jumpFlag;				//Flag to alert at what point the jump feature is at. False = on the ground, True = in the air
	float jumpTime;				//Time being calculated as of the initiation of the jump
	float camRadius;			//Distance maitained by the camera when dealing with circular objects
	float headBob;				//Maitains time of movement in order to generate the headbob
	float groundHeight;			//Average Heigth of nearby ground
	
};
