#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct vertexData{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 color;
	glm::vec2 UV;
};

struct textureData{
	unsigned int id;
	unsigned int type;
};

class mesh {
public:
	mesh(vector<vertexData>* vd, vector<unsigned int>* id, vector<textureData>* td=NULL);
	~mesh();
	void draw(unsigned int programId);
private:
	vector<vertexData> data;
	vector<textureData> textures;
	vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int IND;
};