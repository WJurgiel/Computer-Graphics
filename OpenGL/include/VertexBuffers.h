#pragma once
#include <vector>
#include "WindowSetup.h"
class VertexBuffers
{
private:
	unsigned VAO, VBO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	void generateVAO() {
		glGenVertexArrays(1, &VAO);
	}
	void generateVBO(GLenum usage) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);
	}
	void generateEBO(GLenum usage) {
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], usage);
	}
protected:
	VertexBuffers(std::vector<float>& vertices, std::vector<int>& indices, GLenum usage) {
		this->vertices = vertices;
		this->indices = indices;
		generateVAO();
		bindVAO();
		generateVBO(usage);
		generateEBO(usage);
		unbindVAO();
	}
	
	void bindVAO() {
		glBindVertexArray(VAO);
	}
	void unbindVAO() {
		glBindVertexArray(0);
	}
	void updateVBO(std::vector<float> vertices, GLenum usage) {
		//sprawdzic czy vertexy aktualizuj¹ siê w klasie wy¿ej
		this->vertices = vertices;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], usage);
	}
	void updateEBO(std::vector<int> indices, GLenum usage) {
		this->indices = indices;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], usage);
	}
	/*
	* !stride and pointer are just plain numbers, function handles multiplying them by the type, and casting
	*/
	void vertexAttribPointer(GLenum id, GLint size, GLenum type, GLboolean normalized, GLint stride, GLint pointer) {
		bindVAO();
		glVertexAttribPointer(id, size, type, normalized, stride * sizeof(type), (void*)(pointer * sizeof(type)));
		glEnableVertexAttribArray(id);
		unbindVAO();
	}
	void freeMemory() {
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	

};

