#include "GLEW/glew.h"
#include "DemoSquare.h"
#include "CameraControl.h"

DemoSquare::DemoSquare()
{
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

DemoSquare::~DemoSquare() {}

void DemoSquare::draw(unsigned int shader, CameraControl camera)
{
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

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}