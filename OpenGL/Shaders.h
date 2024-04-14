#pragma once
const GLchar* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"layout(location = 1) in vec2 texture;\n"
"out vec2 vertexTexture;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y,position.z, 1.0);\n"
"vertexTexture = texture;\n"
"}\0";

const GLchar* fragmentShaderSource =
"#version 330 core\n"
"in vec2 vertexTexture;\n"
"uniform vec3 uColor = vec3(1.0,1.0,1.0);\n"
"uniform sampler2D uTexture;\n"
"out vec4 fragmentColor;\n"
"void main()\n"
"{\n"
" fragmentColor = mix(texture(uTexture, vertexTexture), vec4(uColor.x, uColor.y/2, uColor.z/3, 1.0), 0.5);\n"
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