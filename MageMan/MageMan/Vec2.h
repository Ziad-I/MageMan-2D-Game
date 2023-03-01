#pragma once
#include <iostream>

class Vec2
{
public:
	float x, y;

	Vec2();
	Vec2(float x, float y);

	Vec2& add(const Vec2& v);
	Vec2& subtract(const Vec2& v);
	Vec2& multiply(const Vec2& v);
	Vec2& divide(const Vec2& v);
	Vec2& scale(float s);
	Vec2& zero();
	Vec2& normalize();
	float dist(const Vec2& v);
	float length() const;

	Vec2& operator+(const Vec2& v1);
	Vec2& operator-(const Vec2& v1);
	Vec2& operator/(const Vec2& v1);
	Vec2& operator*(const Vec2& v1);

	bool operator==(const Vec2& v1);
	bool operator!=(const Vec2& v1);

	void operator+=(const Vec2& v);
	void operator-=(const Vec2& v);
	void operator/=(const Vec2& v);
	void operator*=(const Vec2& v);

	friend std::ostream& operator<<(std::ostream& stream, const Vec2& v);

	bool isNegative();
};
