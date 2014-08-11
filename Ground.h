#pragma once

#include "OBJLoader.h"
#include "Model.h"
#include "Renderer.h"

class Ground : public Model {
public:
	Ground();
	virtual ~Ground();

	virtual void update(float dt);
	virtual void draw();
private:
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	OBJLoader* model;
};