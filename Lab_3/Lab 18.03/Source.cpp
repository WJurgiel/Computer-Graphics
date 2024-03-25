#include <iostream>
#include "WindowSetup.h"
#include "Shaders.h"
#include <vector>
#include <math.h>

int N;
std::vector<float> vertices;
std::vector<unsigned> indices;

void createCircle(float r, float N) {
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


int main() {
	float radius = 1;
	std::cout << "Podaj liczbe trojkatow N (N>8) : ";
	do {
		std::cin >> N;
	} while (N < 8);
	std::cout << "r = " << radius;
	createCircle(radius, N);

	for (auto x : indices) {
		std::cout << x << " ";
	}
	for (auto x : vertices) {
		std::cout << x << " ";
	}
	WindowSetup w(500, 500, "Lab_3", NULL, NULL);
	glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);


	unsigned vertexShader = *vertexShaderCompiler(vertexShaderSource);
	unsigned fragmentShader = *fragmentShaderCompiler(fragmentShaderSource);
	unsigned shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
	detachShaders(shaderProgram, vertexShader, fragmentShader);

	glUseProgram(shaderProgram);

	unsigned VAO, VBO, EBO;
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

	while (!glfwWindowShouldClose(w.window)) {
		glClearColor(0.7f, 0.6f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(w.window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}