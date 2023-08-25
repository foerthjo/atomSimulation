#pragma once

#include "CameraControl.h"

class Sphere {
public:
	static const int width = 32, height = 16;
	float vertices[32 * 16 * 3];
	unsigned int indices[31 * 15 * 6];

	unsigned int vertexBufferID;
	unsigned int indexBufferID;

	Sphere();
	~Sphere();

	void draw(unsigned int shader, CameraControl camera);
};