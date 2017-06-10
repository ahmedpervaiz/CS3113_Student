#pragma once

#include <math.h>
#include "Vector3.h"

class Point {
public:
	Point();
	Point(float x, float y);
	Point(Vector3 v);

	Point & Normal();
	void Normalize();
	float Dot(const Point & other);

	Point & operator+=(const Point & other);
	Point & operator-=(const Point & other);
	friend Point & operator+(const Point & lhs, const Point & rhs);
	friend Point & operator-(const Point & lhs, const Point & rhs);

	float x;
	float y;
};