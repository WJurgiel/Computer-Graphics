#include <iostream>
#include "WindowSetup.h"
#include "Shaders.h"
#include "stb_image.h"

float vertices[] = {
    //coords                    //texture coords
    -0.7f, 0.7f, 0.0f,         0.0f, 1.0f,      //TL
    -0.7f, 0.3f, 0.0f,        0.0f, 0.0f,      //BL
    -0.3f, 0.3f, 0.0f,         1.0f, 0.0f,      //BR
    -0.3f, 0.7f, 0.0f,          1.0f, 1.0f      //TR
};
float vertices2[] = {
    0.2f, -0.8f, 0.f,   0.5f, 1.0f,
    0.5f, -0.8f, 0.f,   0.0f, 0.0f,
    0.35f, -0.6f, 0.f, 1.0, 0.0f
};
unsigned indices[] = {
    0,1,2,
    0,3,2
};

int disp_mode = 1;
float col = 0;
unsigned shaderProgram;
unsigned shaderProgram2;
unsigned uColorLocation;
unsigned uColorLocation2;
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    col += yoffset;
    if (col > 1) col = 1;
    
    glUseProgram(shaderProgram);
    glUniform3f(uColorLocation, col, col, col);
    
    glUseProgram(shaderProgram2);
    glUniform3f(uColorLocation2, col, col, col);
    std::cout << "Col: " << col << "\n";
}
void keyboardEvents(GLFWwindow* window, unsigned uColorLocation) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        disp_mode = 1;
        //glUniform3f(uColorLocation, 1.f, 0.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        disp_mode = 2;
        //glUniform3f(uColorLocation, 0.f, 1.f, 0.f);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        disp_mode = 3;
        //glUniform3f(uColorLocation, 0.f, 0.f, 1.f);
    }
}
int main()
{
    WindowSetup w(500, 500, "Texturing", NULL, NULL);

    glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);
    glfwSetKeyCallback(w.window, keyboardCallback);
    glfwSetScrollCallback(w.window, scroll_callback);

    /*
        BUFFERS
    */
    // --------------------------------------------------------------------------
    unsigned VAO, VAO2, VBO, EBO, VBO2;
    
    //SQUARE
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //TRIANGLE
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //zmieniæ potem 3 na 5 po dodaniu text coords
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // --------------------------------------------------------------------------
    /*
        SHADERS
    */
    
    // --------------------------------------------------------------------------
    unsigned vertexShader = *vertexShaderCompiler(vertexShaderSource);
    unsigned fragmentShader = *fragmentShaderCompiler(fragmentShaderSource);
     shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
     shaderProgram2 = *shaderProgramCreator(vertexShader, fragmentShader);
    detachShaders(shaderProgram, vertexShader, fragmentShader);
    detachShaders(shaderProgram2, vertexShader, fragmentShader);

    uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
    uColorLocation2 = glGetUniformLocation(shaderProgram2, "uColor");
    
    // not gonna test if it would work on the first location in memory
    // --------------------------------------------------------------------------
    /*
        TEXTURES
    */
    int tex_width, tex_height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte* data = stbi_load("text1.png", &tex_width, &tex_height, &nrChannels, 0);
    GLuint texture[2];
    //TEXTURE 1
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load text1\n";
    }
    
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //TEXTURE 2
    data = stbi_load("text2.png", &tex_width, &tex_height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load text1\n";
    }
    
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glUseProgram(shaderProgram);

    glEnable(GL_TEXTURE_2D);
    while(!glfwWindowShouldClose(w.window)){
        keyboardEvents(w.window, uColorLocation);


        glClearColor(0.4f, 0.3f, 0.7f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        switch (disp_mode) {

        case 1: 
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glUseProgram(shaderProgram2);
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        case 3:
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned), GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glUseProgram(shaderProgram2);
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }
        

        glfwPollEvents();
        glfwSwapBuffers(w.window);
    }
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);
    glfwTerminate();

}
