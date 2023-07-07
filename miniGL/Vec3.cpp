#include "Graphic.h"
#include <cmath>
#include "Matrix3.h"
#include <vector>
#include "Vec3.h"

Vec3::Vec3()
{
	v[0] = 0.0; v[1] = 0.0; v[2] = 0.0;
}

Vec3::Vec3(double x, double y)
{
	v[0] = x; v[1] = y; v[2] = 1.0;
}

Vec3::Vec3(double x, double y, double z)
{
	v[0] = x; v[1] = y; v[2] = z;
}

void Vec3::set(double x, double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

double Vec3::Length()
{
	return sqrt(this->v[0] * this->v[0] + this->v[1] * this->v[1] + this->v[2] * this->v[2]);
}

Vec3 Vec3::operator+(const Vec3& rhs)
{
	return Vec3(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2]);
}

Vec3 Vec3::operator-(const Vec3& rhs)
{
	return Vec3(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2]);
}

Vec3 Vec3::operator-=(const Vec3& rhs)
{
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}

Vec3 Vec3::operator+=(const Vec3& rhs)
{
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this;
}

Vec3& Vec3::operator*=(Matrix3& m)
{
	*this = *this * m;
	return *this;
}

Vec3 Vec3::operator*(Matrix3& m)
{
	return Vec3(v[0] * m.a() + v[1] * m.c() + v[2] * m.l(), v[0] * m.b() + v[1] * m.d() + v[2] * m.m(), v[0] * m.p() + v[1] * m.q() + v[2] * m.s());
}

double Vec3::operator*(Vec3& m)
{
	return double(v[0] * m.v[0] + v[1] * m.v[1] + v[2] * m.v[2]);
}

Vec3 Vec3::operator^(const Vec3& rhs) const
{
	return Vec3(v[1] * rhs.v[2] - v[2] * rhs.v[1], v[2] * rhs.v[0] - v[0] * rhs.v[2], v[0] * rhs.v[1] - v[1] * rhs.v[0]);
}

Vec3 Vec3::operator-()
{
	return Vec3(-v[0], -v[1], -v[2]);
}

Vec3 Vec3::operator*(double rhs) const
{
	return Vec3(rhs * v[0], rhs * v[1], rhs * v[2]);
}

Vec3 Vec3::operator/(double rhs) const
{
	return Vec3(v[0] / rhs, v[1] / rhs, v[2] / rhs);
}

Vec3 Vec3::operator+(double rhs) const
{
	return Vec3(rhs + v[0], rhs + v[1], rhs + v[2]);
}

Vec3 Vec3::operator-(double rhs) const
{
	return Vec3(rhs - v[0], rhs - v[1], rhs - v[2]);
}

Vec3& Vec3::operator*=(double rhs)
{
	v[0] *= rhs;
	v[1] *= rhs;
	v[2] *= rhs;
	return *this;
}

Vec3& Vec3::operator/=(double rhs)
{
	v[0] /= rhs;
	v[1] /= rhs;
	v[2] /= rhs;
	return *this;
}

double Vec3::Distance(Vec3& lhs, Vec3& rhs)
{
	return sqrt((lhs.v[0] - rhs.v[0]) * (lhs.v[0] - rhs.v[0]) + (lhs.v[1] - rhs.v[1]) * (lhs.v[1] - rhs.v[1]) + (lhs.v[2] - rhs.v[2]) * (lhs.v[2] - rhs.v[2]));
}

Vec3 Vec3::normalize()
{
	return  Vec3(v[0] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]), v[1] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]), v[2] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
}


