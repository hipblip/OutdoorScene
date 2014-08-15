#pragma once

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <map>

#include "OBJLoader.h"
#include "Model.h"
#include "Renderer.h"

class Ground : public Model {
public:
	Ground();
	virtual ~Ground();
	
	virtual void update(float dt);
	virtual void draw();

	void setHmScale(glm::vec3 scale);
	glm::vec3 getHmScale() { return hmScale; };
	float getHeight(float x, float z);
private:
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	glm::vec3 hmScale;
	std::map<std::pair<float, float>, float> scaledHeight;

	unsigned char* LoadBmp(char* filename, BITMAPINFOHEADER *bmpInfoHeader);

	GLuint vertexArrayID;
	GLuint indexVertexBufferID;
	GLuint hmdataVertexBufferID;
	GLuint uvBufferID;
	GLuint normalBufferID;

	bool loadGroundTexture(string path);
	int textureWidth;
	int textureHeight;
	int textureBPP;
	string texturePath;
	GLuint textureID;
	GLuint samplerID;

	bool loadHeightMap(string path);
	int hmCols;
	int hmRows;

	vector<BYTE> data;
	int dataSize;
	int currentDataSize;
	void addData(void* ptr, GLuint dataSize);

	vector<BYTE> iData;
	int indexSize;
	int currentIndexSize;
	void addIndexData(void* ptr, GLuint dataSize);
	//GLuint hmBufferID;

	//OBJLoader* model;
};