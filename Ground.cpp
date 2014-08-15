/*

This class uses a height map stored in a grey scaled bitmap file to create the terrain for the scene.  Indexed drawing is used to alter the heights at the vertices of the terrain.

Author: Samuel Desranleau

*/

#include "Ground.h"
#include <FreeImage.h>

//using namespace glm;

/* The ground uses indexed drawing to draw the ground in an efficient way.  2 buffers are created, one for the vertices and one for the indices which point to a specific vertex.
 */
Ground::Ground() {
	hmScale = glm::vec3(1000.f, 200.f, 1000.f);
	std::cout << loadGroundTexture("Models/sandGrass.jpg");
	loadHeightMap("Models/test1.bmp");

	std::cout << "height at 5, 5: " << getHeight(5.f, 5.f);
}

Ground::~Ground() {
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &hmdataVertexBufferID);
	glDeleteBuffers(1, &indexVertexBufferID);
	glDeleteTextures(1, &textureID);
	glDeleteSamplers(1, &samplerID);
}

/*
	Loads a heightmap from a grey scaled image which must have square dimensions.

	@param the path to the file
	@return whether the file was correctly loaded or not
*/
bool Ground::loadHeightMap(string path) {

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);

	fif = FreeImage_GetFileType(path.c_str(), 0);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	}

	if (fif == FIF_UNKNOWN) {
		std::cout << "Unknown Filetype\n";
		return false;
	}

	if (FreeImage_FIFSupportsReading(fif)) {
		dib = FreeImage_Load(fif, path.c_str());
	}

	if (!dib) {
		std::cout << "Unable to load height map\n";
		return false;
	}

	BYTE* dataPointer = FreeImage_GetBits(dib);
	hmRows = FreeImage_GetHeight(dib);
	hmCols = FreeImage_GetWidth(dib);

	// How much to increase data pointer to get to next pixel data
	unsigned int ptr_inc = FreeImage_GetBPP(dib) == 24 ? 3 : 1;

	// Length of one row in data
	unsigned int row_step = ptr_inc * hmCols;

	glGenBuffers(1, &hmdataVertexBufferID);

	vector<vector<glm::vec2>> UVcoordinates(hmRows, vector<glm::vec2>(hmCols)); 
	vector<vector<glm::vec3>> vertices(hmRows, vector<glm::vec3>(hmCols));

	float textureU = float(hmCols * 0.1);
	float textureV = float(hmRows * 0.1);

	// Calculate vertex and texture data from the value of the color in the height map
	for (int i = 0; i < hmRows; i++) {
		for (int j = 0; j < hmCols; j++) {
			float x = float(j) / float(hmCols - 1);
			float z = float(i) / float(hmRows - 1);
			float y = float(*(dataPointer + row_step * i + j * ptr_inc)) / 255.f;
			vertices[i][j] = glm::vec3(-.5 + x, y, -.5 + z);
			UVcoordinates[i][j] = glm::vec2(textureU * x, textureV * z);
			scaledHeight.insert(std::pair<std::pair<float, float>, float>(std::pair<float, float>(x, z), y));
		}
	}
	
	// Calculate the normals by getting the normals of both triangles in a quad and storing them in a 3 dimensional vector
	vector<vector<glm::vec3>> triangleNormals[2]; // Every quad has 2 triangles
	for (int i = 0; i < 2; i++) {
		triangleNormals[i] = vector<vector<glm::vec3>>(hmRows - 1, vector<glm::vec3>(hmCols - 1));
	}

	// iterate through every quad and calculate the normals
	for (int i = 0; i < hmRows - 1; i++){
		for (int j = 0; j < hmCols - 1; j++) {
			glm::vec3 triangle0[] = {vertices[i][j], vertices[i+1][j], vertices[i+1][j+1]};
			glm::vec3 triangle1[] = {vertices[i+1][j+1], vertices[i][j+1], vertices[i][j]};
			glm::vec3 triangle0Norm = glm::cross(triangle0[0] - triangle0[1], triangle0[1] - triangle0[2]);
			glm::vec3 triangle1Norm = glm::cross(triangle1[0] - triangle1[1], triangle1[1] - triangle1[2]);
			triangleNormals[0][i][j] = glm::normalize(triangle0Norm);
			triangleNormals[1][i][j] = glm::normalize(triangle1Norm);
		}
	}

	// Calculate the normal of every vertex by taking the average of each adjacent triangle's normal
	vector<vector<glm::vec3>> vertexNormals = vector<vector<glm::vec3>>(hmRows, vector<glm::vec3>(hmCols));

	for (int i = 0; i < hmRows; i++) {
		for (int j = 0; j < hmCols; j++) {

			glm::vec3 norm(0,0,0);
			
			if (i != 0 && j != 0) {
				for(int	k = 0; k < 2; k++) {
					norm += triangleNormals[k][i-1][j-1];
				}
			}
			if (i != (hmRows - 1) && j != (hmCols - 1)) {
				for (int k = 0; k < 2; k++) {
					norm += triangleNormals[k][i][j];
				}
			}
			if (i != 0 && j != (hmCols - 1)) {
				norm += triangleNormals[0][i-1][j];
			}
			if (i != (hmRows - 1) && j != 0 ) {
				norm += triangleNormals[1][i][j-1];
			}

			norm = glm::normalize(norm);

			vertexNormals[i][j] = norm;
		}
	}

	// Create the buffer for the indexed drawing
	glGenBuffers(1, &hmdataVertexBufferID);
	data.reserve(hmRows * hmCols * (2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	dataSize = hmRows * hmCols * (2 * sizeof(glm::vec3) + sizeof(glm::vec2));
	currentDataSize = 0;

	glGenBuffers(1, &hmdataVertexBufferID);

	for (int i = 0; i < hmRows; i++) {
		for (int j = 0; j < hmCols; j++) {
			addData(&vertices[i][j], sizeof(glm::vec3));
			addData(&UVcoordinates[i][j], sizeof(glm::vec2));
			addData(&vertexNormals[i][j], sizeof(glm::vec3));
		}
	}

	glGenBuffers(1, &indexVertexBufferID);
	indexSize = 0;
	currentIndexSize = 0;

	int restartIndex = hmRows * hmCols;
	
	for (int i = 0; i < hmRows - 1; i++) {
		for (int j = 0; j < hmCols; j++) {
			for (int k = 0; k < 2; k++) {
				int row = i + (1 - k);
				int index = row * hmCols + j;
				addIndexData(&index, sizeof(int));
			}
		}
		addIndexData(&restartIndex, sizeof(int));
	}
	
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, hmdataVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, data.size(), &data[0], GL_STATIC_DRAW);
	


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVertexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.size(), &iData[0], GL_STATIC_DRAW);

	return true;
}

/*
	Returns the height of the terrain at a certain coordinate
	@param the x and z coordinates
	@return the y coordinate corresponding to the x and z coordinates provided
*/
float Ground::getHeight(float x, float z) {
	float unscaledX = x / hmScale.x;
	float unscaledZ = z / hmScale.z;

	int tempx = unscaledX * 1000;
	int tempz = unscaledZ * 1000;

	unscaledX = float(tempx / 1000);
	unscaledZ = float (tempz / 1000);

	float unscaledY = scaledHeight.at(std::pair<float, float>(unscaledX, unscaledZ));
	float height = unscaledY * hmScale.y;

	return height;
}

/*
	Sets the scale of the map
	@param the desired scale
	@return void
*/
void Ground::setHmScale(glm::vec3 scale) {
	hmScale = glm::vec3(scale);
}

void Ground::addIndexData(void* ptr, GLuint dataSize) {
	iData.insert(iData.end(), (BYTE*)ptr, (BYTE*) ptr + dataSize);
	currentIndexSize += dataSize;
}

void Ground::addData(void* ptr, GLuint dataSize) {
	data.insert(data.end(), (BYTE*) ptr, (BYTE*) ptr + dataSize);
	currentDataSize += dataSize;
}

/*
	Loads a texture froma file to be applied to the terrain
	@param the path to the texture file
	@return whether or not the file was loaded correctly
*/
bool Ground::loadGroundTexture(string path) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* texture(0);
	fif = FreeImage_GetFileType(path.c_str(), 0);

	if (fif == FIF_UNKNOWN) { // Unknown file type
		//std::cout << "Unknown Filetype\n";
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	}

	if (fif == FIF_UNKNOWN) { // Still unkown file type
		std::cout << "Unknown Filetype\n";
		return false;
	}

	if (FreeImage_FIFSupportsReading(fif)) { // is the file supported by free image?
		texture = FreeImage_Load(fif, path.c_str());
	}

	if (!texture) {
		std::cout << "This file type is not supported by FreeImage\n";
		return false;
	}

	BYTE* dataPointer = FreeImage_GetBits(texture);

	textureWidth = FreeImage_GetWidth(texture);
	textureHeight = FreeImage_GetHeight(texture);
	textureBPP = FreeImage_GetBPP(texture);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	int format = textureBPP == 24 ? GL_BGR : textureBPP == 8 ? GL_LUMINANCE : 0;
	int internalFormat = textureBPP == 24 ? GL_BGR : GL_DEPTH_COMPONENT;
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, dataPointer);

	glGenerateMipmap(GL_TEXTURE_2D);	

	FreeImage_Unload(texture);

	glGenSamplers(1, &samplerID);

	texturePath = path;

	// Tri linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

// The ground shouldn't need any animations
void Ground::update(float dt) {
}

/* Draws the ground according to the height map provided
 * @param void
 * @return void
 */
void Ground::draw() {
	Renderer::setShader(2);//set current shader to the ground shader
	glUseProgram(Renderer::getShader());

	glm::mat4 view = glm::lookAt(glm::vec3(0, 250, 1000), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	GLuint viewID = glGetUniformLocation(Renderer::getShader(), "viewMatrix");
	GLuint projID = glGetUniformLocation(Renderer::getShader(), "projectionMatrix");
	GLuint worldID = glGetUniformLocation(Renderer::getShader(), "worldMatrix");
	GLuint TextureID  = glGetUniformLocation(Renderer::getShader(), "myTextureSampler");
	GLuint hmScaleID = glGetUniformLocation(Renderer::getShader(), "scaleMatrix");

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), hmScale);

	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projID, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(worldID, 1, GL_FALSE, &getWorldMatrix()[0][0]);
	glUniformMatrix4fv(hmScaleID, 1, GL_FALSE, &scaleMat[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(TextureID, 0);

	glBindVertexArray(vertexArrayID);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(hmRows * hmCols);
	
	int iNumIndices = (hmRows-1)*hmCols*2 + hmRows-1;
	glDrawElements(GL_TRIANGLE_STRIP, iNumIndices, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLE_STRIP, HM_SIZE_X*(HM_SIZE_Y-1)*2+HM_SIZE_Y-2, GL_UNSIGNED_INT, 0);
}