#include <glad/glad.h>
#include <glfw3.h>
#include "WindowSetup.h"
#include "Shaders.h"

//GLfloat vertices[] = {
//	0.5f,0.5f,0.0f,	//TR
//	0.5f, -0.5f, 0.0f,	//BR
//	-0.5f, -0.5f, 0.0f,	//BL
//	-0.5f, 0.5f, 0.0f //TL
//};
float vertices[] = {
	0.5f,0.5f,0.0f, 1.0f, 0.0f, 0.0f, //top right 0
	0.5f,-0.5f,0.0f, 1.0f, 1.0f, 0.0f,//bottom right 1
	-0.5f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f,//bottom left 2
	-0.5f,0.5f,0.0f, 1.0f, 0.0f, 1.0f //top left 3
};
GLfloat indices[] = {
	0,1,3,
	1,2,3
};
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"vertexColor = color;\n"
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
	GLuint* vs = new GLuint;
	*vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vs, 1, &source, NULL);
	glCompileShader(*vs);
	getShaderCompilationStatus(*vs, "Vertex");
	return vs;
}
GLuint* fragmentShaderCompiler(const GLchar* source) {
	GLuint* fs = new GLuint;
	*fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fs, 1, &source, NULL);
	glCompileShader(*fs);
	getShaderCompilationStatus(*fs, "Fragment");
	return fs;
}
GLuint* shaderProgramCreator(GLuint& vertexShader, GLuint& fragmentShader) {
	GLuint* sp = new GLuint;
	*sp = glCreateProgram();
	glAttachShader(*sp, vertexShader);
	glAttachShader(*sp, fragmentShader);
	glLinkProgram(*sp);
	getShaderLinkingStatus(*sp);
	return sp;
}
void detachShaders(GLuint& shaderProgram, GLuint& vertexShader, GLuint& fragmentShader) {
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
int main() {
	WindowSetup w(600, 500, "Test", NULL, NULL);
	//Shaders s("vertex1.txt", "fragment1.txt");
	glfwSetFramebufferSizeCallback(w.window, &WindowSetup::framebuffer_size_callback);
	
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position attribute
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color attribute
	glEnableVertexAttribArray(1);
	
	
	//Vertex shader
	GLuint*	vertexShader = vertexShaderCompiler(vertexShaderSource);
	//GLuint vertexShader2 = *vertexShaderCompiler(vertexShaderSource2);
	//fragment shader
	GLuint* fragmentShader = fragmentShaderCompiler(fragmentShaderSource);
	//Shader Program
	GLuint* shaderProgram = shaderProgramCreator(*vertexShader, *fragmentShader);

	detachShaders(*shaderProgram, *vertexShader, *fragmentShader);


		glUseProgram(*shaderProgram);
	/*MAIN LOOP*/
	while (!glfwWindowShouldClose(w.window)) {
		glClearColor(1.f, 0.0f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(*shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(w.window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(*shaderProgram);
	delete vertexShader, fragmentShader, shaderProgram;
	glfwTerminate();
}