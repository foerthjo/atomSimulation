#pragma once

#include <GLFW/glfw3.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Rotation.h"

class CameraControl
{
public:
	Vector3 position = Vector3(-1, 0, 0);
	Rotation rotation = Rotation(Vector3::forward(), Vector3::up());
	bool middleMouseDown = false;
	Vector2 mouseStartPosition;
	double sensitivity = .01;
	double orbitDistance = 1;

	float nearPlane = .1;
	float farPlane = 100;

	void handleCursorMotion(GLFWwindow* window, double xpos, double ypos);
	void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
	void zoom(double delta);
	void update();
};