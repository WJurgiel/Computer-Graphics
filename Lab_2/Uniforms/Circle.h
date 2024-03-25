#pragma once
#include <vector>
#include "WindowSetup.h"
class Circle
{
private:
	unsigned VAO, VBO, EBO;
	float r;
	int N;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
public:
	Circle(float r) : r(r) {
		setBuffers();
	}
	Circle(float r, float n) : r(r), N(n){
		createCircle();
		setBuffers();
	}
	~Circle() {
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
	void setBuffers() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	void setN(int N) {
		this->N = N;
	}
	void changeCircle(int N) {
		this->N = N;
		destroyCircle();
		createCircle();
	}
	void destroyCircle() {
		vertices.clear();
		indices.clear();
	}
	void createCircle() {
		float phi = 360.f / N;
		int triangleCount = N - 2;


		for (int i = 0; i < N; i++) {
			float currentPhi = phi * i;
			float x = r * cos(currentPhi * (3.14f / 180.f));
			float y = r * sin(currentPhi * (3.14f / 180.f));
			float z = 0.0f;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		for (int i = 0; i < triangleCount; i++) {
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
	}
	void drawCircle() 
	{

		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), &indices[0], GL_STATIC_DRAW);
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	
};

