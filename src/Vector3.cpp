#include "Vector3.h"
#include <cmath>

Vector3 Vector3::getSubtracted(Vector3 v) {
	return { x - v.x, y - v.y, z - v.z };
}

Vector3 Vector3::getAdded(Vector3 v) {
	return { x + v.x, y + v.y, z + v.z };
}

float Vector3::getScalar(Vector3 v) {
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::getNormalized() {
	float l = this->getLength();
	if (l == 0)
		return up();
	return { x / l, y / l, z / l };
}

Vector3 Vector3::getNormalized(double unit) {
	double l = this->getLength();
	if (l == 0)
		return up(unit);
	l = unit / l;
	return { x * l, y * l, z * l };
}

float Vector3::getSqrLength() {
	return x * x + y * y + z * z;
}

float Vector3::getLength() {
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::getVectorProduct(Vector3 v) {
	return { y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y };
}

Vector3 Vector3::getMultiplied(double val) {
	return { x * val, y * val, z * val };
}

Vector3 Vector3::lerp(Vector3 from, Vector3 to, double t) {
	double t1 = 1 - t;
	return { from.x * t1 + to.x * t, from.y * t1 + to.y * t , from.z * t1 + to.z * t };
}

Vector3 Vector3::up() {
	return { 0, 1, 0 };
}
Vector3 Vector3::up(double length) {
	return { 0, length, 0 };
}
Vector3 Vector3::right() {
	return { 1, 0, 0 };
}
Vector3 Vector3::forward() {
	return { 0, 0, 1 };
}
Vector3 Vector3::zero() {
	return { 0, 0, 0 };
}
Vector3 Vector3::one() {
	return { 1, 1, 1 };
}