#include "Rotation.h"

Rotation::Rotation(Vector3 look, Vector3 up) {
	this->look(look, up);
}

void Rotation::look(Vector3 look, Vector3 up) {
	forward = look.getNormalized();
	right = look.getVectorProduct(up).getMultiplied(-1).getNormalized();
	this->up = right.getVectorProduct(forward).getMultiplied(-1).getNormalized();
}