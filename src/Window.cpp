#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "ShaderTools.h"

#include "Vector3.h"
#include "Rotation.h"

#include "CameraControl.h"

#include "DemoSquare.h"
#include "AtomSimulation.h"

static CameraControl camera;

static void handleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	camera.handleMouseButton(window, button, action, mods);
}

static void handleCursorMotion(GLFWwindow* window, double x, double y)
{
	camera.handleCursorMotion(window, x, y);
}

static void scrollCallback(GLFWwindow* window, double x, double y)
{
	camera.zoom(y);
}

static void errorCallback(int c, const char* message) {
	std::cout << message << "\n";
}

int main(void) {
	glfwSetErrorCallback((GLFWerrorfun) errorCallback);

	if (!glfwInit())
		return -1;

	// use monitor for fullscreen
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	GLFWwindow* window = glfwCreateWindow(768, 768, "Atom Simulation", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW ERROR! Press ENTER to exit." << std::endl;
		std::cin.get();
		glfwTerminate();
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// compute shader
	unsigned int velocityUpdateShader = ShaderTools::createComputeShaderFromFile("res/velocityUpdateShader.glsl");
	unsigned int positionUpdateShader = ShaderTools::createComputeShaderFromFile("res/positionUpdateShader.glsl");

	// input
	glfwSetCursorPosCallback(window, handleCursorMotion);
	glfwSetMouseButtonCallback(window, handleMouseButton);
	glfwSetScrollCallback(window, (GLFWscrollfun) scrollCallback);

	// draw normal stuff
	glRenderMode(GL_RENDER);

	AtomSimulation* atom = new AtomSimulation(100, 157, 100);

	Vector3 cameraPosition = { 0, 0, -1 };
	Rotation cameraRotation = Rotation(cameraPosition.getMultiplied(-1), Vector3::up());

	unsigned int demoSquareShader = ShaderTools::createGeometryShaderFromFile("res/basicShader.glsl");
	unsigned int atomShader = ShaderTools::createGeometryShaderFromFile("res/instancedSphere.glsl");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glEnableVertexAttribArray(0);

	glfwSwapInterval(1);

	// glEnable(GL_CULL_FACE);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		camera.update();
		atom->update(velocityUpdateShader, positionUpdateShader);

		// rendering
		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// drawcalls
		atom->draw(atomShader, camera);

		// swap buffers
		glfwSwapBuffers(window);

		// poll and process events
		glfwPollEvents();
	}

	delete atom;

	glDeleteProgram(demoSquareShader);
	glDeleteProgram(atomShader);

	glDeleteProgram(velocityUpdateShader);
	glDeleteProgram(positionUpdateShader);

	glfwTerminate();
	return 0;
}
