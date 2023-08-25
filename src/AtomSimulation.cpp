#include <GLEW/glew.h>

#include "AtomSimulation.h"

#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

AtomSimulation::AtomSimulation(unsigned int protons, unsigned int neutrons, unsigned int electrons) {
	particleCount = protons + neutrons + electrons;

	masses = (float*)malloc(sizeof(float) * 1 * particleCount);
	charges = (float*)malloc(sizeof(float) * 1 * particleCount);
	positions = (float*)malloc(sizeof(float) * 4 * particleCount);
	velocities = (float*)malloc(sizeof(float) * 4 * particleCount);
	scales = (float*)malloc(sizeof(float) * 1 * particleCount);
	colors = (float*)malloc(sizeof(float) * 4 * particleCount);

	for (int i = 0; i < protons; i++) {
		charges[i] = 1;
		masses[i] = 1;
		
		positions[i * 4 + 0] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 1] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 2] = ((double)rand() / RAND_MAX) - .5;

		velocities[i * 4 + 0] = 0;
		velocities[i * 4 + 1] = 0;
		velocities[i * 4 + 2] = 0;

		scales[i] = .1;

		colors[i * 4 + 0] = .5;
		colors[i * 4 + 1] = 0;
		colors[i * 4 + 2] = 0;
	}

	for (int i = protons; i < protons + neutrons; i++) {
		charges[i] = 0;
		masses[i] = 1;

		positions[i * 4 + 0] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 1] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 2] = ((double)rand() / RAND_MAX) - .5;

		velocities[i * 4 + 0] = 0;
		velocities[i * 4 + 1] = 0;
		velocities[i * 4 + 2] = 0;

		scales[i] = .1;

		colors[i * 4 + 0] = .8;
		colors[i * 4 + 1] = .8;
		colors[i * 4 + 2] = .8;
	}

	for (int i = protons + neutrons; i < particleCount; i++) {
		charges[i] = -1;
		masses[i] = 0.05;

		positions[i * 4 + 0] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 1] = ((double)rand() / RAND_MAX) - .5;
		positions[i * 4 + 2] = ((double)rand() / RAND_MAX) - .5;

		velocities[i * 4 + 0] = 0;
		velocities[i * 4 + 1] = 0;
		velocities[i * 4 + 2] = 0;

		scales[i] = .02;

		colors[i * 4 + 0] = 0;
		colors[i * 4 + 1] = .8;
		colors[i * 4 + 2] = .8;
	}

	for (int u = 0; u < width; u++) {
		for (int v = 0; v < height; v++) {
			float y = cos((float)v / (height - 1) * M_PI);
			float inset = cos(asin(y));
			float x = cos((float)u / (width - 1) * 2 * M_PI) * inset;
			float z = sin((float)u / (width - 1) * 2 * M_PI) * inset;
			vertices[(u + v * width) * 3 + 0] = x;
			vertices[(u + v * width) * 3 + 1] = y;
			vertices[(u + v * width) * 3 + 2] = z;
		}
	}

	for (int u = 0; u < width - 1; u++) {
		for (int v = 0; v < height - 1; v++) {
			unsigned int topLeft = u + v * width;
			unsigned int topRight = (u + 1) + v * width;
			unsigned int bottomLeft = u + (v + 1) * width;
			unsigned int bottomRight = (u + 1) + (v + 1) * width;
			indices[(u + v * (width - 1)) * 6 + 0] = topLeft;
			indices[(u + v * (width - 1)) * 6 + 1] = bottomRight;
			indices[(u + v * (width - 1)) * 6 + 2] = topRight;
			indices[(u + v * (width - 1)) * 6 + 3] = topLeft;
			indices[(u + v * (width - 1)) * 6 + 4] = bottomLeft;
			indices[(u + v * (width - 1)) * 6 + 5] = bottomRight;
		}
	}

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &translationBufferID);
	glGenBuffers(1, &scaleBufferID);
	glGenBuffers(1, &colorBufferID);

	glGenBuffers(1, &massBufferID);
	glGenBuffers(1, &chargeBufferID);

	glGenBuffers(1, &velocityBufferID);

	glGenBuffers(1, &outputBufferID);
}

AtomSimulation::~AtomSimulation() {
	free(positions);
	free(scales);
	free(colors);
	free(masses);
	free(charges);
	free(velocities);

	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	glDeleteBuffers(1, &translationBufferID);
	glDeleteBuffers(1, &scaleBufferID);
	glDeleteBuffers(1, &colorBufferID);

	glDeleteBuffers(1, &massBufferID);
	glDeleteBuffers(1, &chargeBufferID);

	glDeleteBuffers(1, &velocityBufferID);

	glDeleteBuffers(1, &outputBufferID);
}

void AtomSimulation::update(unsigned int velocityUpdateShader, unsigned int positionUpdateShader) {
	// update velocities
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, translationBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, positions, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, translationBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, massBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 1 * particleCount, masses, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, massBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, chargeBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 1 * particleCount, charges, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, chargeBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, velocities, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, velocityBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBufferID);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, outputBufferID);

		glUseProgram(velocityUpdateShader);
		glDispatchCompute(particleCount, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBufferID);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 4 * particleCount, velocities);
	}

	// update positions
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, translationBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, positions, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, translationBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, velocities, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocityBufferID);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBufferID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 4 * particleCount, positions, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, outputBufferID);

		glUseProgram(positionUpdateShader);
		glDispatchCompute(particleCount, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBufferID);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 4 * particleCount, positions);
	}

	glRenderMode(GL_RENDER);
}

void AtomSimulation::draw(unsigned int shader, CameraControl& camera) {
	glUseProgram(shader);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	glUniform3f(glGetUniformLocation(shader, "cameraPosition"), camera.position.x, camera.position.y, camera.position.z);
	glUniform3f(glGetUniformLocation(shader, "cameraForward"), camera.rotation.forward.x, camera.rotation.forward.y, camera.rotation.forward.z);
	glUniform3f(glGetUniformLocation(shader, "cameraRight"), camera.rotation.right.x, camera.rotation.right.y, camera.rotation.right.z);
	glUniform3f(glGetUniformLocation(shader, "cameraUp"), camera.rotation.up.x, camera.rotation.up.y, camera.rotation.up.z);

	glUniform1f(glGetUniformLocation(shader, "near"), camera.nearPlane);
	glUniform1f(glGetUniformLocation(shader, "far"), camera.farPlane);

	// instancing
	{
		// vec4 translation at location 1
		glBindBuffer(GL_ARRAY_BUFFER, translationBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * particleCount, positions, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glVertexAttribDivisor(1, 1);

		// float scale at location 2
		glBindBuffer(GL_ARRAY_BUFFER, scaleBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleCount, scales, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), nullptr);
		glVertexAttribDivisor(2, 1);

		// vec4 color at location 3
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * particleCount, colors, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glVertexAttribDivisor(3, 1);
	}

	glDrawElementsInstanced(GL_TRIANGLES, (width - 1) * (height - 1) * 2 * 3, GL_UNSIGNED_INT, nullptr, particleCount);
}