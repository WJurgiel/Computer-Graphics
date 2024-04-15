#include "WindowSetup.h"
#include "Shaders.h"
#include "Square.h"
#include "FileHandler.h"
#include "TexturingUnit.h"
#define WINDOW_HEIGHT 700
#define WINDOW_WIDTH 700
const std::string VERTEX_PATH = "squareVtx.txt";
const std::string INDICES_PATH = "squareIdx.txt";

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

/*
	lab 5:
	*4 trojkaty 
	* rozne kolory 
	* 1 ruch w linii prostej
	* 2 rotacja
	* 3 naprzemiennie zwiêkszenia i zmniejszanie rozmiaru
	* 4 po³¹czenie powy¿szych
*/

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	
	
	WindowSetup w(WINDOW_WIDTH, WINDOW_HEIGHT, "Translations", NULL, NULL);
	glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);
	glfwSetKeyCallback(w.window, keyboardCallback);
	

	std::vector<float> vertices = read_content_from_file<float>(VERTEX_PATH);
	std::vector<int> indices = read_content_from_file<int>(INDICES_PATH);
	
	TexturingUnit tex("cat2.jpg", 0, GL_TEXTURE_2D, GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	tex.initializeTexture();


	Square tr1(vertices, indices, glm::vec3{-0.6f, 0.6f, 0.0f}, std::vector<std::vector<int>>{ {0, 3, 5, 0}, { 1,2,5,3 }},
		vertexShaderSource,
		fragmentShaderSource, tex.texture );
	Square tr2(vertices, indices, glm::vec3{ 0.6f, 0.6f, 0.0f }, std::vector<std::vector<int>>{ {0, 3, 5, 0}, { 1,2,5,3 }},
		vertexShaderSource,
		fragmentShaderSource, tex.texture);
	Square tr3(vertices, indices, glm::vec3{ -0.6f, -0.6f, 0.0f }, std::vector<std::vector<int>>{ {0, 3, 5, 0}, { 1,2,5,3 }},
		vertexShaderSource,
		fragmentShaderSource, tex.texture);
	Square tr4(vertices, indices, glm::vec3{ 0.6f, -0.6f, 0.0f }, std::vector<std::vector<int>>{ {0, 3, 5, 0}, { 1,2,5,3 }},
		vertexShaderSource,
		fragmentShaderSource, tex.texture);
	tr1.changeColor(1.0f, 1.0f, 1.0f);
	tr1.setID(1);
	tr2.changeColor(0.0f, 1.0f, 0.0f);
	tr2.setID(2);
	tr3.changeColor(0.0f, 0.0f, 1.0f);
	tr3.setID(3);
	tr4.changeColor(0.0f, 0.0f, 0.0f);
	tr4.setID(4);
	/*Square sq2(
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
		);*/


	while (!glfwWindowShouldClose(w.window)) {
		
		glClearColor(1.0f, 0.0f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		tex.bindTexture();
		tr1.draw();
		tr1.animate(glfwGetTime(), 0.0001);
		tr2.draw();
		tr2.animate(glfwGetTime(), 0.00001);
		tr3.draw();
		tr3.animate(glfwGetTime(), 0.0001);
		tr4.draw();
		tr4.animate(glfwGetTime(), 0.0001);
		tex.unbindTexture();
		
		glfwPollEvents();
		glfwSwapBuffers(w.window);
	}
	glfwTerminate();
	return 0;
}

