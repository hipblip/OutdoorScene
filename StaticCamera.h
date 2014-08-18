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


class StaticCamera : public Camera
{
public:
	StaticCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector, Ground* inGround);
	virtual ~StaticCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

	virtual glm::vec3 GetLookAt() const;

	virtual glm::vec3 GetPosition() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mLookAtPoint;
	glm::vec3 mUpVector;

	//Added Atributes
	float movement;
	float modifier;
	bool camFlag;
	float radius;
	float groundHeight;
};
