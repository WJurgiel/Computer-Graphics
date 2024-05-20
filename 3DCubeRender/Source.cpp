#include "WindowSetup.h"
#include "FileHandler.h"
#include "TexturingUnit.h"
#include "Cube.h"

static constexpr auto WINDOW_WIDTH = 700.f;
static constexpr auto WINDOW_HEIGHT = 700.f;

float pitch = 0.0f;
float yaw = -90.f;
bool first_window_enter = true;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

const GLchar* vertexShaderSrc = R"(
	#version 330 core
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec3 normal;
	out vec2 vertexTexture;
	out vec3 normalMatrix;
	out vec3 fragmentPosition;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{

	gl_Position = projection * view *model * vec4(position, 1.0);


	fragmentPosition = vec3(model*vec4(position,1.0));
	normalMatrix = mat3(transpose(inverse(model))) * normal;
	}
)";

const GLchar* fragmentShaderSrc = R"(
	//flaga by wylaczyc swiatlo, wylaczyc diffuse, wylaczyc specular
	#version 330 core
	
	//flagi
	uniform int specularOn = 1;
	uniform int diffuseOn = 1;
	uniform int ambientOn = 1;
	in vec2 vertexTexture;
	in vec3 normalMatrix;
	in vec3 fragmentPosition;

	uniform vec3 uColor = vec3(0.0,0.1,1.0);
	uniform sampler2D uTexture;
	uniform vec3 lightPosition;

	//diffuse
	vec3 lightColor = vec3(1.0,1.0,1.0);
	vec3 normalNormalized = normalize(normalMatrix);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuse = max(dot(normalNormalized, lightDirection), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	
	//specular
	uniform vec3 viewPosition;

	out vec4 fragmentColor;
	void main()
	{
		float ambientStrength;
		if(ambientOn == 1) ambientStrength = 0.15;
		else if(ambientOn == 0) ambientStrength = 0.0;
		
		vec3 ambientColor = ambientStrength * lightColor;

		float specularStrength;
		if(specularOn == 1)
			specularStrength = 0.8;
		else if (specularOn == 0)
			specularStrength = 0.0;
		vec3 viewDirection = normalize(viewPosition - fragmentPosition);
		vec3 reflectDirection = reflect(-lightDirection, normalNormalized);
		float specular = pow(max(dot(viewDirection, reflectDirection), 0.0), 64);
		vec3 specularColor = specularStrength * specular * lightColor;

		if(diffuseOn == 1)
		fragmentColor = vec4(uColor * (ambientColor + diffuseColor + specularColor), 1.0);
		if(diffuseOn == 0)
		fragmentColor = vec4(uColor * (ambientColor + specularColor), 1.0);

	}
)";


const GLchar* lightvertexShaderSrc = R"(
	#version 330 core
	layout(location = 0) in vec3 position;
	layout(location = 1) in vec2 texture;
	out vec2 vertexTexture;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
	//gl_Position = model * vec4(position,1.0);
	gl_Position = projection * view *model * vec4(position, 1.0);
	vertexTexture = texture;
	}
)";
const GLchar* lightFragmentShaderSrc = R"(
	#version 330 core
	out vec4 fragmentColor;
	void main()
	{
	  fragmentColor = vec4(1.0,1.0,1.0,1.0);
	}
)";

float deltaTime(GLFWwindow* w) {
	static float previousTime = glfwGetTime();
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - previousTime;

	static int frameCount = 0;
	static float elapsedTime = 0.0f;
	frameCount++;
	elapsedTime += deltaTime;
	if (elapsedTime >= 1.0f) {
		float fps = static_cast<float>(frameCount) / elapsedTime;
		std::cout << "FPS: " << fps << std::endl;

		std::stringstream ss;
		double msDeltaTime = deltaTime * 1000.0f;
		ss << "FPS: " << fps << "| deltaTime:" << deltaTime;
		glfwSetWindowTitle(w, ss.str().c_str());

		frameCount = 0;
		elapsedTime = 0.0f;
	}

	previousTime = currentTime;

	return deltaTime;
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

	//std::cout << xpos << "," << ypos << "\n";
	float previousX = static_cast<float>(WINDOW_WIDTH) / 2.0f;
	float previousY = static_cast<float>(WINDOW_HEIGHT) / 2.0f;
	/*if (first_window_enter) {
		previousX = xpos;
		previousY = ypos;
		first_window_enter = false;
	}*/

	float xdifference = xpos - previousX;
	float ydifference = previousY - ypos;
	previousX = xpos;
	previousY = ypos;

	const float sensitivity = 0.05f * deltaTime(window);
	xdifference *= sensitivity;
	ydifference *= sensitivity;

	pitch += ydifference;
	yaw += xdifference;
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 cameraFront_new;
	cameraFront_new.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront_new.y = sin(glm::radians(pitch));
	cameraFront_new.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraFront_new);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
}
int main() {
	WindowSetup w(WINDOW_WIDTH, WINDOW_HEIGHT, "3D cube render", NULL, NULL);
	w.setWindowPos(800, 200);
	glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(w.window, key_callback);

	std::vector<float> vertices = read_content_from_file<float>("vertices2.txt"); //light vertices
	std::vector<int> indices = read_content_from_file<int>("indices.txt");	
	std::vector<float> normalsVertices = read_content_from_file<float>("normalsVertices.txt");
	TexturingUnit tex("cat2.jpg", 0, GL_TEXTURE_2D, GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	tex.initializeTexture();

	for (auto x : normalsVertices) {
		std::cout << x << "\n";
	}

	Cube cube(normalsVertices, indices, glm::vec3{ 0.f, 0.f,-0.5f },
		std::vector <std::vector<int>>{ {0, 3, 6, 0}, { 1,3,6,3 }},
		vertexShaderSrc, fragmentShaderSrc, tex.texture
		);
	Cube light(vertices, indices, glm::vec3{ 0, 1, - 0.5 },
		std::vector<std::vector<int>>{ {0, 3, 5, 0}, { 1,2,5,3 }},
		lightvertexShaderSrc, lightFragmentShaderSrc, tex.texture
	);
	cube.changeColor(0.1, 0.9, 0.1);
	light.scale(glm::vec3{ 0.5, 0.5, 0.5 });
	
	
	glfwSetCursorPosCallback(w.window, mouseCallback);
	
	/*print pos*/
	int a = 0;
	
	/*
		LIGHT
	*/

	glfwSetTime(0);
	float dt;
	float lastClick = 0.0;
	std::cout << "light sProgram: " << light.shaderProgram << "\n";
	std::cout << "cube sProgram: " << cube.shaderProgram << "\n";
	while (!glfwWindowShouldClose(w.window)) {
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0,0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		dt = deltaTime(w.window);
		lastClick += dt;

		tex.bindTexture();
		cube.draw_noEBO();
		light.draw();

		//cube.cubeMove(deltaTime(w.window));
		cube.cameraMove(w.window, 0.003, cameraFront);
		light.cameraMove(w.window, 0.003, cameraFront);

		light.cubeMove(dt);
		cube.solidCubeMove(w.window, 1,dt);
		
		cube.updateLightUniforms(light.getPosition(), light.getView());
		if (lastClick >= 0.3f) cube.handleLighting(w.window,lastClick);
		glfwPollEvents();
		glfwSwapBuffers(w.window);
	}
}