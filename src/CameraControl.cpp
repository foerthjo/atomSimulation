#include "CameraControl.h"
#include <math.h>

void CameraControl::handleCursorMotion(GLFWwindow* window, double x, double y) {
	if (middleMouseDown)
	{
		Vector2 mousePosition = Vector2(x, y);
		Vector2 delta = mousePosition.getSubtracted(mouseStartPosition).getMultiplied(sensitivity);
		rotation.look(rotation.forward.getAdded(rotation.right.getMultiplied(delta.x)).getAdded(rotation.up.getMultiplied(-delta.y)), rotation.up);
		position = rotation.forward.getMultiplied(-orbitDistance);
		mouseStartPosition = mousePosition;
	}
}

void CameraControl::handleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS) {
			middleMouseDown = true;
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			mouseStartPosition = Vector2(x, y);
		}

		if (action == GLFW_RELEASE) {
			middleMouseDown = false;
		}
	}
}

void CameraControl::zoom(double delta) {
	this->orbitDistance *= pow(1.1, -delta);
}

void CameraControl::update() {
	position = rotation.forward.getMultiplied(-orbitDistance);
}