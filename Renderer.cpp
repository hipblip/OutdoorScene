#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "Renderer.h"
#include "EventManager.h"
// Include GLFW - OpenGL Framework (Multi Platform framework to Setup a window and OpenGL Context
#include <GLFW/glfw3.h>

// Include GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLFWwindow* Renderer::window = NULL;
std::vector<GLuint> Renderer::shaderProgramID;
GLuint Renderer::currentShader;

/* Initializes Glew, sets the color of the background, activates the depth test and loads the shaders into the vector of programID's 
 */
void Renderer::setUp() {
	window = EventManager::getWindow();
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");		
		getchar();
		exit(-1);
	}

	// Black background
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	
	// Enable depth test
    glEnable(GL_DEPTH_TEST);
    
	// Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

	//Load the shaders
	//shaderProgramID.push_back(LoadShaders("Shaders/Phong.vertexshader", "Shaders/Phong.fragmentshader"));
	

	/*Renderer::shaderProgramID.push_back(LoadShaders("Shaders/Phong.vertexshader", "Shaders/Phong.fragmentshader"));*/

	Renderer::shaderProgramID.push_back(LoadShaders("Shaders/modelImport.vertexshader", "Shaders/modelImport.fragmentshader"));	 // index 0
	Renderer::shaderProgramID.push_back(LoadShaders("Shaders/SolidColor.vertexshader","Shaders/SolidColor.fragmentshader"));	 // index 1
	Renderer::shaderProgramID.push_back(LoadShaders("Shaders/groundShader.vertexshader","Shaders/groundShader.fragmentshader")); // index 2

	currentShader = 0;
}

/* This function sets the current shader being used in the the program
 * @param the position of the required shader within the vector of saders
 * @return void
 */
void Renderer::setShader(GLuint value) {
	if (value > (shaderProgramID.size() - 1) || value < 0) {
		std::cout << "Invalid shader ID\n";
	} else {
		currentShader = value;
	}
}

/* A simple getter for the shader
 * @param void
 * @return the current shader
 */
GLuint Renderer::getShader() {
	return shaderProgramID[currentShader];
}

/* Deletes the entries in the vector of shaders and nullifies the window variable.  This is called at the end of the program when the user exits.
 * @param void
 * @return void
 */
void Renderer::terminateProgram() {
	window = NULL;
	for (std::vector<GLuint>::iterator it = shaderProgramID.begin(); it < shaderProgramID.end(); it++) {
		glDeleteProgram(*it);
	}
	shaderProgramID.clear();
}

/* Clears the color and depth buffers.  This is called at the beginning of every frame.
 * @param void
 * @return void
 */
void Renderer::beginFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Swaps the buffers.  Called at the end of every frame.
 * @param void
 * @return void
 */
void Renderer::endFrame() {
	glfwSwapBuffers(window);
}

//
// The following code is taken from
// www.opengl-tutorial.org
//
GLuint Renderer::LoadShaders(const char * vertex_shader_path,const char * fragment_shader_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path);
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}