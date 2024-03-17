#pragma once
#include <fstream>
#include <glad/glad.h>
#include <glfw3.h>
#include <string>
#include <iostream>
#include <sstream>

/*
	1. read vertex shader src
	2. read fragment shader src
	3. create program //
	4. attach and delete //for user
	5. check for compilation problems // private
*/
class Shaders
{
public:
	//the program ID
	unsigned int ID;
	Shaders(const char* vertexPath, const char* fragmentPath);
	void use();
private:
	void checkShaderCompilation(unsigned int shader);
};


