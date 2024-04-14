#pragma once
#include "stb_image.h"
#include "WindowSetup.h"
/*
	flipped vertically on default

*/
class TexturingUnit
{
private:
	int tex_width, tex_height, nrChannels;
	const char* src;
	GLubyte* data;
	GLenum target;
	GLint filterS, filterT, filterMin, filterMag;
	GLint imgFormat; 
	int border;

	void flip_vertically_on_load(bool flag_should_flip);
	void generateTexture();
	void setTextureFiltering();
	void createTexture2D();
public:
	GLuint texture;
	TexturingUnit(std::string source, int desiredChannels, 
		GLenum target, GLint imgFormat,
		GLint filterS, GLint filterT, GLint filterMin, GLint filterMag) {

		this->src = source.c_str();
		this->target = target;
		this->imgFormat = imgFormat;
		this->filterS = filterS;
		this->filterT = filterT;
		this->filterMin = filterMin;
		this->filterMag = filterMag;
		this->border = 0;

		
		data = stbi_load(src, &tex_width, &tex_height, &nrChannels, desiredChannels);
		flip_vertically_on_load(true);
	}
	~TexturingUnit() {
		unbindTexture();
	}
	void bindTexture() {
		glBindTexture(target, texture);
	}
	void unbindTexture() {
		glBindTexture(target, 0);
	}
	
	void initializeTexture() {
		generateTexture();
		bindTexture();
		createTexture2D();
		setTextureFiltering();
		enableTexture();
		unbindTexture();
	}
	
	void enableTexture() {
		glEnable(target);
	}

};

inline void TexturingUnit::flip_vertically_on_load(bool flag_should_flip) {
	stbi_set_flip_vertically_on_load(flag_should_flip);
}
inline void TexturingUnit::generateTexture() {
	glGenTextures(1, &texture);
}
inline void TexturingUnit::setTextureFiltering() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filterS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filterT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
}
inline void TexturingUnit::createTexture2D() {
	if (data) {
		glTexImage2D(target, 0, imgFormat, tex_width, tex_height, border, imgFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(target);
	}
	else {
		std::cout << "Failed to load texture from source: " << src << "\n";
	}
	stbi_image_free(data);
}