#include <iostream>
#include "WindowSetup.h"
#include "Shaders.h"
#include <vector>
#include <math.h>

int N;
unsigned VAO, VBO, EBO;
std::vector<std::vector<float>> colors = {
    {1.0f,0.f,0.f},
    {0.f, 1.f, 0.f},
    {0.f,0.f,1.f}
};
std::vector<float> vertices;
std::vector<unsigned> indices;

void createCircle(float r, int N) {
    vertices.clear();
    indices.clear();

    float phi = 360.f / N;
    int triangleCount = N - 2;

    for (int i = 0; i < N; i++) {
        float currentPhi = phi * i;
        float x = r * cos(currentPhi * (3.14f / 180.f));
        float y = r * sin(currentPhi * (3.14f / 180.f));
        float z = 0.0f;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }
    for (int i = 0; i < triangleCount; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}
void processInput(GLFWwindow* window, unsigned uLocation) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glUniform3f(uLocation, colors[0][0], colors[0][1], colors[0][2]);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glUniform3f(uLocation, colors[1][0], colors[1][1], colors[1][2]);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glUniform3f(uLocation, colors[2][0], colors[2][1], colors[2][2]);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    N += static_cast<int>(yoffset);
    if (N < 8) N = 8;
    std::cout << "N: " << N << "\n";
    createCircle(1.0f, N);

    // Update vertex buffer object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Update index buffer object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

int main() {
    float radius = 1;
    std::cout << "Podaj liczbe trojkatow N (N>8) : ";
    do {
        std::cin >> N;
    } while (N < 8);
    std::cout << "r = " << radius;
    createCircle(radius, N);

    for (auto x : indices) {
        std::cout << x << " ";
    }
    for (auto x : vertices) {
        std::cout << x << " ";
    }
    WindowSetup w(500, 500, "Lab_3", NULL, NULL);
    glfwSetFramebufferSizeCallback(w.window, w.framebuffer_size_callback);
    glfwSetScrollCallback(w.window, scroll_callback);

    unsigned vertexShader = *vertexShaderCompiler(vertexShaderSource);
    unsigned fragmentShader = *fragmentShaderCompiler(fragmentShaderSource);
    unsigned shaderProgram = *shaderProgramCreator(vertexShader, fragmentShader);
    detachShaders(shaderProgram, vertexShader, fragmentShader);

    glUseProgram(shaderProgram);


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), &indices[0], GL_STATIC_DRAW);


    unsigned uColorLocation = glGetUniformLocation(shaderProgram, "uColor");

    while (!glfwWindowShouldClose(w.window)) {
        glClearColor(0.3f, 0.1f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(w.window, uColorLocation);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(w.window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
