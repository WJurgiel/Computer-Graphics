#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "WindowSetup.h"
float vertices[] = {
	0.5f,0.5f,0.0f, 1.0f, 0.0f, 0.0f, //top right 0
	0.5f,-0.5f,0.0f, 1.0f, 1.0f, 0.0f,//bottom right 1
	-0.5f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f,//bottom left 2
	-0.5f,0.5f,0.0f, 1.0f, 0.0f, 1.0f //top left 3
};
float vertices2[] = {
	-0.2f, -0.2f, 0.0f, 0.5f, 0.5f, 0.7f,
	-0.2f, -0.7f, 0.0f, 0.6f, 0.1f, 0.3f,
	-0.7f, -0.7f, 0.0f, 0.1f, 0.2f, 0.3f,
	-0.7f, -0.2f, 0.0f, 0.9f, 0.3f, 0.4f
};
unsigned int indices[] = {
	0,1,3,
	1,2,3
};
unsigned int indicesTask2[] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0
};

const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"//layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y,position.z, 1.0);\n"
"vertexColor = vec3(0.7,0.3, 0.2);\n"
"}\0";
const GLchar* vertexShaderSource2 =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"//layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y,position.z, 1.0);\n"
"vertexColor = vec3(0.1,0.7, 0.3);\n"
"}\0";
const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 fragmentColor;\n"
"void main()\n"
"{\n"
" fragmentColor = vec4(vertexColor, 1.0);\n"
"}\0";

void getShaderCompilationStatus(GLint shader, std::string type) {
	GLint status;
	GLchar error_message[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(shader, 512, NULL, error_message);
		std::cout << "[Error] " << type << " shader: " << error_message << std::endl;
	}
}
void getShaderLinkingStatus(GLuint shaderProgram) {
	GLint status;
	GLchar error_message[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, error_message);
		std::cout << "Error (shader program): " << error_message << std::endl;
	}
}
GLuint* vertexShaderCompiler(const GLchar* source) {
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &source, NULL);
	glCompileShader(vs);
	getShaderCompilationStatus(vs, "Vertex");
	return &vs;
}
GLuint* fragmentShaderCompiler(const GLchar* source) {
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &source, NULL);
	glCompileShader(fs);
	getShaderCompilationStatus(fs, "Fragment");
	return &fs;
}
GLuint* shaderProgramCreator(GLuint& vertexShader, GLuint& fragmentShader) {
	GLuint sp = glCreateProgram();
	glAttachShader(sp, vertexShader);
	glAttachShader(sp, fragmentShader);
	glLinkProgram(sp);
	getShaderLinkingStatus(sp);
	return &sp;
}
void detachShaders(GLuint& shaderProgram, GLuint& vertexShader, GLuint& fragmentShader) {
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	std::cout << width << ", " << height << std::endl;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
int main() {
	unsigned w_height = 500;
	unsigned w_width = 600;
	WindowSetup w(500, 600, "H", NULL, NULL);
	glfwSetFramebufferSizeCallback(w.window, framebuffer_size_callback);
	/*glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Witam", NULL, NULL);
	if (!window) {
		std::cout << "Failed" << "\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize glad\n";
		return -1;
	}

	glViewport(0, 0, w_width, w_height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);*/


	//VAOs, VBOs, EBOs:
	unsigned int VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);
	
	//FIRST SQUARE
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//SECOND SQUARE
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices, GL_STATIC_DRAW);*/
	//Element Buffer Object
	
	
	//Vertex shader
	GLuint	vertexShader = *vertexShaderCompiler(vertexShaderSource);
	GLuint vertexShader2 = *vertexShaderCompiler(vertexShaderSource2);
	//fragment shader
	GLuint fragmentShader = *fragmentShaderCompiler(fragmentShaderSource);
	//Shader Program
	GLuint shaderProgram[2] = {
		*shaderProgramCreator(vertexShader, fragmentShader),
		*shaderProgramCreator(vertexShader2, fragmentShader) };
	
	detachShaders(shaderProgram[0], vertexShader, fragmentShader);
	

	glUseProgram(shaderProgram[0]);


	while (!glfwWindowShouldClose(w.window)) {
		processInput(w.window);

		glClearColor(0.3f, 0.1f, 0.51f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAOs[0]); //it's just to make things organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAOs[1]); //it's just to make things organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*default: */

		/*Task 2*/
		//glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(w.window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	/*glDeleteBuffers(1, &VBO2);*/
	glDeleteBuffers(2, EBOs);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);
	glfwTerminate();
	return 0;
}