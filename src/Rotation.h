#pragma once

#include "Vector3.h"

struct Rotation {
	Vector3 right, up, forward;

	Rotation(Vector3 look, Vector3 up);
	void look(Vector3 look, Vector3 up);
};