#pragma once
#include "VertexBuffers.h"
#include "Shaders.h"
#include "GLM/glm/glm.hpp"
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>


class Cube : public VertexBuffers, public Shader {
private:
	std::vector<float> vertices;
	std::vector<int> indices;
	unsigned vertexShader;
	unsigned fragmentShader;
	unsigned uLocation;
	glm::mat4 model;
	GLint modelLocation;
	GLuint texture;

	/// <summary>
	/// Camera stuff, put it to another class after classes
	/// </summary>
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 2.5f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection;
	const float cameraSpeed = 0.03f;
public:
	
	unsigned shaderProgram;
	Cube(std::vector<float> vertices, std::vector<int> indices, glm::vec3 transVec, std::vector<std::vector<int>> attribPointers,
		const GLchar* vertexShaderSrc, const GLchar* fragmentShaderSrc, GLuint texture) :
		VertexBuffers(vertices, indices, GL_STATIC_DRAW) {
		this->vertices = vertices;
		this->indices = indices;

		for (int i = 0; i < attribPointers.size(); i++)
			vertexAttribPointer(
				attribPointers[i][0], //id
				attribPointers[i][1], //size
				GL_FLOAT,
				GL_FALSE,
				attribPointers[i][2], //stride
				attribPointers[i][3] //pointer
			);

		/*SHADERS*/
		vertexShader = *vertexShaderCompiler(vertexShaderSrc);
		fragmentShader = *fragmentShaderCompiler(fragmentShaderSrc);
		shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
		detachShaders(shaderProgram, vertexShader, fragmentShader);
		glUseProgram(shaderProgram);
		uLocation = glGetUniformLocation(shaderProgram, "uColor");

		/*TEXTURES*/
		this->texture = texture;

		//TRANSFORMATIONS:
		
		model = glm::mat4(1.0f);
		translate(transVec);

		camera_perspective();

		int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
		
		
		
		glUseProgram(0);

	}
	~Cube() {
		freeMemory();
	}
	void camera_view(glm::vec3 cameraFront) {
		glUseProgram(shaderProgram);
		view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view"); 
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUseProgram(0);
	}
	void camera_perspective() {
		glUseProgram(shaderProgram);
		projection = glm::perspective(glm::radians(45.0f), 
			700.f/700.f,
			0.1f, 100.0f);
		GLint viewLoc = glGetUniformLocation(shaderProgram, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUseProgram(0);
	}
	void translate(glm::vec3 transVector) {
		glUseProgram(shaderProgram);
		model = glm::translate(model, transVector);
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUseProgram(0);
	}
	void scale(glm::vec3 scaleVector) {
		glUseProgram(shaderProgram);
		model = glm::scale(model, scaleVector);
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUseProgram(0);
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
	void cameraMove(GLFWwindow* window, const float cameraSpeed, glm::vec3 cameraFront) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition += cameraSpeed * cameraFront; 
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		camera_view(cameraFront);
		//std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << "\n";
	}
	void solidCubeMove(GLFWwindow* window, float speed, float dT) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) translate(glm::vec3{ 0.f,0.f,dT * speed });
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  translate(glm::vec3{ 0.f,0.f,-dT * speed });
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  translate(glm::vec3{ dT * speed ,0.f,0.f});
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) translate(glm::vec3{ -dT * speed ,0.f,0.f });
		//camera_view(cameraFront);
		//std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << "\n";
	}
	void cubeMove(float dT) {
		glUseProgram(shaderProgram);
		float angle = 45.f * dT;
	
		model = glm::translate(model, glm::vec3{4*cos(glfwGetTime())*dT, 0.f, 4 * sin(glfwGetTime()) * dT });
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUseProgram(0);
	}
	void updateLightUniforms(glm::vec3 lightPos, glm::vec3 viewPos) {
		glUseProgram(shaderProgram);
		GLuint lightLoc = glGetUniformLocation(shaderProgram, "lightPosition");
		GLuint cameraLoc = glGetUniformLocation(shaderProgram, "viewPosition");
		glUniform3f(lightLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(cameraLoc, viewPos.x, viewPos.y, viewPos.z);
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
	void draw_noEBO() {
		bindVAO();
		if (doesTextureExist()) {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		unbindVAO();
		glUseProgram(0);
	}
	glm::vec3 getPosition() {
		// Extract the translation part of the matrix
		return glm::vec3(model[3][0], model[3][1], model[3][2]);
	}
	glm::vec3 getView() {
		// Extract the translation part of the matrix
		return glm::vec3(view[3][0], view[3][1], view[3][2]);
	}
	void handleLighting(GLFWwindow* window, float& lastClick) {
		if (glfwGetKey(window, GLFW_KEY_T)) {
			glUseProgram(shaderProgram);
			GLuint specularLoc = glGetUniformLocation(shaderProgram, "specularOn");
			GLint uniformValue;
			glGetUniformiv(shaderProgram, specularLoc, &uniformValue);
			if (uniformValue == 1) {
				std::cout << "Specular: OFF\n";
				glUniform1i(specularLoc, 0);
				lastClick = 0.0f;
			}
			else if (uniformValue == 0) {
				std::cout << "Specular: ON\n";
				glUniform1i(specularLoc, 1);
				lastClick = 0.0f;
			}	
			glUseProgram(0);
		}
		if (glfwGetKey(window, GLFW_KEY_Y)) {
			glUseProgram(shaderProgram);
			GLuint diffuseLoc = glGetUniformLocation(shaderProgram, "diffuseOn");
			GLint uniformValue;
			glGetUniformiv(shaderProgram, diffuseLoc, &uniformValue);
			if (uniformValue == 1) {
				std::cout << "Diffuse: OFF\n";
				glUniform1i(diffuseLoc, 0);
				lastClick = 0.0f;
			}
			else if (uniformValue == 0) {
				std::cout << "Diffuse: ON\n";
				glUniform1i(diffuseLoc, 1);
				lastClick = 0.0f;
			}
			glUseProgram(0);
		}
		if (glfwGetKey(window, GLFW_KEY_U)) {
			glUseProgram(shaderProgram);
			GLuint ambientLoc = glGetUniformLocation(shaderProgram, "ambientOn");
			GLint uniformValue;
			glGetUniformiv(shaderProgram, ambientLoc, &uniformValue);
			if (uniformValue == 1) {
				std::cout << "Ambient: OFF\n";
				glUniform1i(ambientLoc, 0);
				lastClick = 0.0f;
			}
			else if (uniformValue == 0) {
				std::cout << "Ambient: ON\n";
				glUniform1i(ambientLoc, 1);
				lastClick = 0.0f;
			}
			glUseProgram(0);
		}
	}

};