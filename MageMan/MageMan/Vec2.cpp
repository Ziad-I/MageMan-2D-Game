#pragma once
#include "Vec2.h"
#include <cmath>

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2& Vec2::add(const Vec2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vec2& Vec2::subtract(const Vec2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vec2& Vec2::divide(const Vec2& v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

Vec2& Vec2::multiply(const Vec2& v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

Vec2& Vec2::scale(float s)
{
	this->x *= s;
	this->y *= s;
	return *this;
}

Vec2& Vec2::zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}

Vec2& Vec2::normalize()
{
	float l = length();
	this->x = this->x / l;
	this->y = this->y / l;
	return *this;
}

float Vec2::dist(const Vec2& v)
{
	return sqrt(abs((v.x - this->x) * (v.x - this->x)) + abs((v.y - this->y) * (v.y - this->y)));
}

float Vec2::length() const
{
	return sqrt(x * x + y * y);
}

Vec2& Vec2::operator+(const Vec2& v1)
{
	return this->add(v1);
}

Vec2& Vec2::operator-(const Vec2& v1)
{
	return this->subtract(v1);
}

Vec2& Vec2::operator*(const Vec2& v1)
{
	return this->multiply(v1);
}

Vec2& Vec2::operator/(const Vec2& v1)
{
	return this->divide(v1);
}

void Vec2::operator+=(const Vec2& v)
{
	this->x += v.x;
	this->y += v.y;
}

void Vec2::operator-=(const Vec2& v)
{
	this->x -= v.x;
	this->y -= v.y;
}

bool Vec2::operator==(const Vec2& v1)
{
	return this->x == v1.x && this->y == v1.y;
}

bool Vec2::operator!=(const Vec2& v1)
{
	return this->x != v1.x || this->y != v1.y;
}

void Vec2::operator*=(const Vec2& v)
{
	this->x *= v.x;
	this->y *= v.y;
}

void Vec2::operator/=(const Vec2& v)
{
	this->x /= v.x;
	this->y /= v.y;
}

std::ostream& operator<<(std::ostream& stream, const Vec2& v)
{
	stream << "(" << v.x << ", " << v.y << ")";
	return stream;
}

bool Vec2::isNegative()
{
	return x < 0 && y < 0;
}