#include "Point.h"

Point::Point()
{
}

Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

Point::Point(Vector3 v)
{
	x = v.x;
	y = v.y;
}

Point & Point::Normal()
{
	float length = sqrtf(x*x + y*y);
	if (length == 0) {
		return Point(0, 0);
	}
	return Point(x / length, y / length);
}

void Point::Normalize()
{
	Point normal = Normal();
	x = normal.x;
	y = normal.y;
}

float Point::Dot(const Point& other) {
	return x*other.x + y*other.y;
}

Point& Point::operator+=(const Point& other) {
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Point& Point::operator-=(const Point& other) {
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

Point& operator+(const Point & lhs, const Point & rhs)
{
	return Point(lhs.x + rhs.x, lhs.y + rhs.y);
}

Point& operator-(const Point & lhs, const Point & rhs)
{
	return Point(lhs.x - rhs.x, lhs.y - rhs.y);
}

