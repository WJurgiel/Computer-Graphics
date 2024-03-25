#pragma once
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y,position.z, 1.0);\n"
"vertexColor = vec3(0.7,0.3, 0.2);\n"
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