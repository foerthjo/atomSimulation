#pragma once

struct Vector3 {
	double x, y, z;

	Vector3()
		: x(0), y(0), z(0) {}

	Vector3(double x, double y, double z)
		: x(x), y(y), z(z) {}

	Vector3 getSubtracted(Vector3 v);
	Vector3 getAdded(Vector3 v);
	float getScalar(Vector3 v);
	Vector3 getNormalized();
	Vector3 getNormalized(double unit);
	float getSqrLength();
	float getLength();
	Vector3 getVectorProduct(Vector3 v);
	Vector3 getMultiplied(double val);

	static Vector3 lerp(Vector3 from, Vector3 to, double t);

	static Vector3 up();
	static Vector3 up(double length);
	static Vector3 right();
	static Vector3 forward();
	static Vector3 zero();
	static Vector3 one();
};