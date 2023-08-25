#pragma once

#include "Vector3.h";
#include "CameraControl.h"

class AtomSimulation {
	int particleCount;

	float* masses;
	float* charges;
	float* positions;
	float* velocities;
	float* scales;
	float* colors;

	static const int width = 32, height = 16;
	float vertices[32 * 16 * 3];
	unsigned int indices[31 * 15 * 6];

	unsigned int vertexBufferID, indexBufferID;

	unsigned int translationBufferID, scaleBufferID, colorBufferID;
	unsigned int massBufferID, chargeBufferID;
	unsigned int velocityBufferID;

	unsigned int outputBufferID;

public:
	AtomSimulation(unsigned int protons, unsigned int neutrons, unsigned int electrons);
	~AtomSimulation();
	void update(unsigned int velocityUpdateShader, unsigned int positionUpdateShader);
	void draw(unsigned int shader, CameraControl& camera);
};