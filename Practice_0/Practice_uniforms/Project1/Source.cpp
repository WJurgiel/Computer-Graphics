#include <iostream>
#include "WindowSetup.h"
#include "Shaders.h"
#include <time.h>
#include <vector>
#include <math.h>
#include <chrono>

WindowSetup w(500, 500, "Uniforms", NULL, NULL);

class Time {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
public:
	Time() {
		lastTime = std::chrono::high_resolution_clock::now();
	}
	float deltaTime() {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		return deltaTime.count();
	}
};
class Player {
private:
	unsigned VAO, VBO, EBO;
	float x, y, z;
	float width, height;
	float move_speed;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	
	void updateVertices() {
		vertices = {
			x - width / 2.f, y - height / 2.f, 0.0f, // BL
			x + width / 2.f, y - height / 2.f, 0.0f, // BR
			x + width / 2.f, y + height / 2.f, 0.0f, //TR
			x - width / 2.f, y + height / 2.f, 0.0f // TL
		};
	}
public:
	Player(float x, float y, float z, float w, float h, float move_speed)
		: x(x), y(y), z(z), width(w), height(h), move_speed(move_speed) {
		
		updateVertices();
		indices = {
			0,1,2,
			2,3,0
		};
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // data from VBO
		glEnableVertexAttribArray(0);

	}
	~Player() {
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
	void move() {
		if (glfwGetKey(w.window, GLFW_KEY_RIGHT)==GLFW_PRESS) x += move_speed;
		if (glfwGetKey(w.window, GLFW_KEY_LEFT) == GLFW_PRESS) x -= move_speed;
		if (glfwGetKey(w.window, GLFW_KEY_UP) == GLFW_PRESS) y += move_speed;
		if (glfwGetKey(w.window, GLFW_KEY_DOWN) == GLFW_PRESS) y -= move_speed;
		updateVertices();
		//std::cout << "[" << x << ", " << y << ", " << z << "]\n";
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		//printVertices();
	}
	void printVertices() {
		for (auto x : vertices) {
			std::cout << x << ", ";
		}
		std::cout << "\n";
	}
	void printIndices() {
		for (auto x : indices) {
			std::cout << x << ", ";
		}
		std::cout << "\n";
	}
	std::vector<float> getVertices() {
		return vertices;
	}
	std::vector<unsigned> getIndices() {
		return indices;
	}
	void draw() {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

std::vector<float> windowColor;
std::vector<float> generateWindowColor() {
	std::vector<float> toReturn = {0.f,0.f,0.f};
	
	for (auto& x : toReturn) {
		x = (float)(rand() % 101 / 100.f);
		std::cout << x << " ";
	}
	std::cout << "\n";
	return toReturn;
}
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) windowColor = generateWindowColor(); // windowColor of type std::vector<float> 
}

int main() {
	srand(time(NULL));
	windowColor = generateWindowColor();
	std::vector<float> cubeColor = { 1.f,0.2f,0.1f };
	
	Player p(0.f, 0.f, 0.f, 0.2f, 0.2f, 0.0001f);
	Time time;
	
	
	glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);
	glfwSetKeyCallback(w.window, keyboardCallback);
	
	unsigned vertexShader = *vertexShaderCompiler(vertexShaderSource);
	unsigned fragmentShader = *fragmentShaderCompiler(fragmentShaderSource);
	unsigned shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
	detachShaders(shaderProgram, vertexShader, fragmentShader);
	GLuint uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
	
	glUseProgram(shaderProgram);
	glUniform3f(uColorLocation,(cubeColor[0]), (cubeColor[1]), (cubeColor[2]));
	std::cout << "V:\n";
	p.printVertices();
	std::cout << "I:\n";
	p.printIndices();
	while (!glfwWindowShouldClose(w.window)) {
		float deltaTime = time.deltaTime();
		glClearColor(windowColor[0], windowColor[1], windowColor[2], 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUniform3f(uColorLocation,
			(cubeColor[0]),
			(cubeColor[1]),
			(cubeColor[2])
		);
		p.move();
		p.draw();
		glfwPollEvents();
		glfwSwapBuffers(w.window);
	}
	
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}