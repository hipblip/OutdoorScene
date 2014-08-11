#pragma once

#include <vector>
#include <sstream>
#include "mesh.h"

#include <Importer.hpp>      
#include <scene.h>           
#include <postprocess.h>   

#define ILUT_USE_OPENGL

#include<IL/il.h>
#include<IL/ilu.h>
#include<IL/ilut.h>

using namespace std;

class OBJLoader {
public:
	OBJLoader(const char* filename);
	~OBJLoader();
	void draw(unsigned int programId);
	vector<mesh*>& getMeshes();
private:
	vector<mesh*> meshes;
	void recursiveProcess(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int loadTexture(const char* filename);
	void devilStartUp();
	string filepath(const char* filename);
};