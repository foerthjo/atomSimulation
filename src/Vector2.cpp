#include "Vector2.h"
#include <cmath>

bool Vector2::equals(Vector2 v) {
	return x == v.x && y == v.y;
}

Vector2 Vector2::getAdded(Vector2 v) {
	return { x + v.x, y + v.y };
}

Vector2 Vector2::getSubtracted(Vector2 v) {
	return { x - v.x, y - v.y };
}

double Vector2::getLength() {
	return sqrt(x * x + y * y);
}

void Vector2::add(Vector2 v) {
	x += v.x;
	y += v.y;
}

Vector2 Vector2::getMultiplied(double val) {
	return { x * val, y * val };
}

void Vector2::multiply(double val) {
	x *= val;
	y *= val;
}

Vector2 Vector2::getNormalized() {
	double l = getLength();
	if (l == 0)
		return { 0, -1 };
	return { x / l, y / l };
}

Vector2 Vector2::getNormalized(double unit) {
	double l = getLength();
	if (l == 0)
		return { 0, -unit };
	return { x * unit / l, y * unit / l };
}

Vector2 Vector2::getRightVector() {
	return { y, -x };
}

double Vector2::getScalar(Vector2 v) {
	return x * v.x + y * v.y;
}