#pragma once

#include <math.h> 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GrassModel.h"
#include<cstdlib>

using namespace std;

GrassModel::GrassModel() {
	int iSign = getRandomSign();
	int iSignDiff = getRandomSign();
	float fRandomX = ( rand() % 100 ) / 10.0;
	float fRandomZ = ( rand() % 100 ) / 10.0;
	float fRandomXDiff = ( rand() % 25 ) / 100.0;
	float fRandomZDiff = getRandomSign() * ( rand() % 100 ) / 10000.0;
	float fRandomGreen = ( rand() % 50 ) / 100.0 + 0.5;
	float fRandomRed = ( rand() % 50 ) / 100.0;
	float angle = ( rand() % 360 );
	mat3 m( cos( angle ), 0.0f, -sin( angle ), 0.0f, 1.0f, 0.0f, sin( angle ), 0.0f, cos( angle ) );
	if( getRandomSign() > 0 ) {
		Vertex vertexBuffer[]  = {
			{ vec3(iSign * fRandomX,                                           0,                               fRandomZ                   ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * 0.01,                        0,                               fRandomZ + fRandomZDiff    ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff + 0.01 ),     0.32 / ( fRandomXDiff + 1 ),     fRandomZ + fRandomZDiff    ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff + 0.02 ),     0.32 / ( fRandomXDiff + 1 ),     fRandomZ + fRandomZDiff * 2), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff * 2 + 0.02 ), 0.56 / ( fRandomXDiff * 2 + 1 ), fRandomZ + fRandomZDiff * 2), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff * 2 + 0.03 ), 0.56 / ( fRandomXDiff * 2 + 1 ), fRandomZ + fRandomZDiff * 3), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff * 3 + 0.04 ), 0.72 / ( fRandomXDiff * 3 + 1 ), fRandomZ + fRandomZDiff * 3), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff * 3 + 0.04 ), 0.72 / ( fRandomXDiff * 3 + 1 ), fRandomZ + fRandomZDiff * 4), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ vec3(iSign * fRandomX + iSignDiff * ( fRandomXDiff * 4 + 0.06 ), 0.96 / ( fRandomXDiff * 4 + 1 ), fRandomZ + fRandomZDiff * 6), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) }
		};
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(Vertex), vertexBuffer, GL_STATIC_DRAW);
	} else {
		fRandomZDiff = ( rand() % 25 ) / 100.0;
		Vertex vertexBuffer[]  = {
			{ m * vec3(iSign * fRandomX,          0,    fRandomZ ),                vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX + 0.01,   0,    fRandomZ ),                vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX,          0.32 / ( fRandomZDiff + 1 ),     fRandomZ + fRandomZDiff + 0.01 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX + 0.0075, 0.32 / ( fRandomZDiff + 1 ),     fRandomZ + fRandomZDiff + 0.01 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX,          0.56 / ( fRandomZDiff * 2 + 1 ), fRandomZ + fRandomZDiff * 2 + 0.02 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX + 0.005,  0.56 / ( fRandomZDiff * 2 + 1 ), fRandomZ + fRandomZDiff * 2 + 0.02 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX,          0.72 / ( fRandomZDiff * 3 + 1 ), fRandomZ + fRandomZDiff * 3 + 0.03 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX + 0.0025, 0.72 / ( fRandomZDiff * 3 + 1 ), fRandomZ + fRandomZDiff * 3 + 0.03 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) },
			{ m * vec3(iSign * fRandomX,          0.96 / ( fRandomZDiff * 4 + 1 ), fRandomZ + fRandomZDiff * 4 + 0.04 ), vec3(0, 0, 0), vec3(fRandomRed, fRandomGreen, 0) }
		};
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(Vertex), vertexBuffer, GL_STATIC_DRAW);
	}
}

GrassModel::~GrassModel() {}

void GrassModel::Update( float dt ) {	
	// @TODO 6 (Optional) - Here is where you can add some code if you want your vehicle to animate
}

int GrassModel::getRandomSign() {
	return rand() % 2 == 1 ? 1 : -1;
}

void GrassModel::Draw() {
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3,	GL_FLOAT, GL_FALSE,	sizeof(Vertex), (void*)0);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(vec3)));

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 9);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	if (glGetError() != GL_NO_ERROR)
	{
		fprintf(stderr, "OpengGL error %d", glGetError());
	}
}
