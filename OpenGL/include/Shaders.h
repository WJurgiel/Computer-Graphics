#pragma once
/*
	MODIFY SQUARE CLASS IN CASE ADDING MORE UNIFORMS

*/


class Shader {
private:
	unsigned uColorLocation;
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
protected:
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
	void setUniform3f(unsigned uLocation, float v1, float v2, float v3) {
		glUniform3f(uLocation, v1, v2, v3);
	}

};


