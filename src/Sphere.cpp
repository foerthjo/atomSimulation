#include "GLEW/glew.h"
#include "Sphere.h"
#include "CameraControl.h"

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere() {
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
}

Sphere::~Sphere() {}

void Sphere::draw(unsigned int shader, CameraControl camera) {
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

	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 2 * 3, GL_UNSIGNED_INT, nullptr);
}