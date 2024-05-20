#pragma once
#include "VertexBuffers.h"
#include "Shaders.h"
#include "GLM/glm/glm.hpp"
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>
/*
Simple Square class
It inherits from classes Shader, and VertexBuffers, so we can say that square class contains its own

*/

class Square : public VertexBuffers, public Shader {
private:
	std::vector<float> vertices;
	std::vector<int> indices;
	unsigned vertexShader;
	unsigned fragmentShader;
	unsigned uLocation;
	GLint modelLocation;
	GLuint texture;
	glm::mat4 model;
public:
	unsigned shaderProgram;
	Square(std::vector<float> vertices, std::vector<int> indices, std::vector<std::vector<int>> attribPointers,
		const GLchar* vertexShaderSrc, const GLchar* fragmentShaderSrc, GLuint texture) :
		VertexBuffers(vertices, indices, GL_STATIC_DRAW) {
		this->vertices = vertices;
		this->indices = indices;

		for(int i = 0; i < attribPointers.size(); i++)
			vertexAttribPointer(
				attribPointers[i][0], //int
				attribPointers[i][1], //size
				GL_FLOAT,
				GL_FALSE,
				attribPointers[i][2],
				attribPointers[i][3] //pointer
			);

		/*SHADERS*/
		vertexShader = *vertexShaderCompiler(vertexShaderSrc);
		fragmentShader = *fragmentShaderCompiler(fragmentShaderSrc);
		shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
		detachShaders(shaderProgram, vertexShader, fragmentShader);
		glUseProgram(shaderProgram);
		uLocation = glGetUniformLocation(shaderProgram, "uColor");

		//TRANSFORMATIONS
		modelLocation = glGetUniformLocation(shaderProgram, "model");
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUseProgram(0);

		/*TEXTURES*/
		this->texture = texture;
	
	}
	~Square() {
		freeMemory();
	}
	void changeColor(float r, float g, float b) {
		glUseProgram(shaderProgram);
		std::cout << uLocation;
		setUniform3f(uLocation, r, g, b);
		glUseProgram(0);
	}
	bool doesTextureExist() {
		return this->texture != NULL;
	}
	void move() {
		glUseProgram(shaderProgram);
		glm::mat4 model = glm::mat4(1.0f);
		//how is this supposed to work, it shouldn't be able to multiply
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		
		glUseProgram(0);
	}
	void draw() {
		bindVAO();
		if (doesTextureExist()) {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glUseProgram(shaderProgram);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		unbindVAO();
		glUseProgram(0);
	}
	


};