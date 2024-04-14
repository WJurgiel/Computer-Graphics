#include "WindowSetup.h"
#include "Shaders.h"
#include "Square.h"
#include "FileHandler.h"
#include "TexturingUnit.h"
#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH 400
const std::string VERTEX_PATH = "squareVtx.txt";
const std::string INDICES_PATH = "squareIdx.txt";
unsigned shaderProgramPlaceHolder;
glm::mat4 model = glm::mat4(1.0f);

//glm::mat4 model = glm::mat4(1.0f);
/*
	1. import vertices and indices by FileHandler::read_content_from_file<type>(path)
	2. It will sound totally lame, but:
		* if you want your object to be textured, use fragmentShaderSource
		* if you want them just to be coloured use fragmentShaderSource2
	Reason: Didn't implement checking if there is a texture in shaders
	3. Create your class object:
		* Inherit from Shader and VertexBuffers class
		* Every object of the world has its own buffers, shaders, textures and transformations,
	4. Remember to carefully input attributes
		*the syntax of attributes matrix is:
			{{id, how many elements to attrib, stride, pointer}, ...}
	5. Remember to add .cpp files of external libraries
*/


void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	double speed = 0.1f;
	model = glm::scale(model, glm::vec3(1.0f + yoffset * speed));
	glUseProgram(shaderProgramPlaceHolder);

	GLint modelLoc = glGetUniformLocation(shaderProgramPlaceHolder, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	std::cout << shaderProgramPlaceHolder;
	glUseProgram(0);
}
int main() {
	
	
	WindowSetup w(WINDOW_WIDTH, WINDOW_HEIGHT, "Translations", NULL, NULL);
	glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);
	glfwSetKeyCallback(w.window, keyboardCallback);
	glfwSetScrollCallback(w.window, scrollCallback);

	std::vector<float> vertices = read_content_from_file<float>(VERTEX_PATH);
	std::vector<int> indices = read_content_from_file<int>(INDICES_PATH);
	
	TexturingUnit tex("cat2.jpg", 0, GL_TEXTURE_2D, GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	tex.initializeTexture();


	Square sq(vertices, indices, std::vector<std::vector<int>>{ {0, 3, 5, 0}, {1,2,5,3}},
		vertexShaderSource,
		fragmentShaderSource, tex.texture );
	Square sq2(
		std::vector<float>{
		-0.9f, 0.9f, 0.0f, 0.0, 1.0f,
			-0.9f, 0.8f, 0.0f, 0.0f, 0.0f,
			-0.8f, 0.8f, 0.0f, 1.0, 0.0f,
			-0.8f, 0.9f, 0.0f, 1.0, 1.0f},
		std::vector<int>{0, 1, 2, 0, 3, 2},
			std::vector<std::vector<int>>{ {0, 3, 5, 0}, {1, 2, 5, 3}},
			vertexShaderSource,
			fragmentShaderSource,
			tex.texture
		);

	shaderProgramPlaceHolder = sq.shaderProgram;

	while (!glfwWindowShouldClose(w.window)) {
		
		glClearColor(1.0f, 0.0f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		tex.bindTexture();
		sq2.draw();
		sq.move(w.window, model);
		sq.rotate(w.window, model, 1.f);
		sq.draw();
		tex.unbindTexture();
		
		glfwPollEvents();
		glfwSwapBuffers(w.window);
	}
	glfwTerminate();
	return 0;
}

