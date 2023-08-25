#pragma once

#include "CameraControl.h"

class DemoSquare {
public:
	float vertices[12] =
	{
		-.5, -.5, 0,
		 .5, -.5, 0,
		 .5,  .5, 0,
		-.5,  .5, 0
	};

	unsigned int indices[6] =
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vertexBufferID;
	unsigned int indexBufferID;

	DemoSquare();
	~DemoSquare();

	void draw(unsigned int shader, CameraControl camera);
};