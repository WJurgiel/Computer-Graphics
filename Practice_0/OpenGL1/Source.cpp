#include <glad/glad.h>
#include <glfw3.h>
#include "WindowSetup.h"
#include "Shaders.h"
int main() {
	WindowSetup w(600, 500, "Test", NULL, NULL);
	
	glfwSetFramebufferSizeCallback(w.window, &WindowSetup::framebuffer_size_callback);

	/*MAIN LOOP*/
	while (!glfwWindowShouldClose(w.window)) {
		glClearColor(0.4f, 0.3f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(w.window);
		glfwPollEvents();
	}
}