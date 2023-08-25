#pragma once

struct Vector2 {
	double x, y;

	Vector2(double x, double y)
		: x(x), y(y) {}

	Vector2()
		: x(0), y(0) {}

	bool equals(Vector2 v);
	Vector2 getAdded(Vector2 v);
	Vector2 getSubtracted(Vector2 v);
	double getLength();
	void add(Vector2 v);
	Vector2 getMultiplied(double val);
	void multiply(double val);
	Vector2 getNormalized();
	Vector2 getNormalized(double unit);
	Vector2 getRightVector();
	double getScalar(Vector2 v);
};