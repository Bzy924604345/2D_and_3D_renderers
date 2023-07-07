#include "Matrix3.h"
#define PI 3.1415926535

Matrix3::Matrix3()
{
	M[0][0] = 1.0, M[0][1] = 0.0, M[0][2] = 0.0,
	M[1][0] = 0.0, M[1][1] = 1.0, M[1][2] = 0.0,
	M[2][0] = 0.0, M[2][1] = 0.0, M[2][2] = 1.0;
}

Matrix3::Matrix3(double a, double b, double p, double c, double d, double q, double l, double m)
{
	M[0][0] = a, M[0][1] = b, M[0][2] = p,
	M[1][0] = c, M[1][1] = d, M[1][2] = q,
	M[2][0] = l, M[2][1] = m, M[2][2] = 1.0;
}
Matrix3::Matrix3(double a, double b, double p, double c, double d, double q, double l, double m, double s)
{
	M[0][0] = a, M[0][1] = b, M[0][2] = p,
	M[1][0] = c, M[1][1] = d, M[1][2] = q,
	M[2][0] = l, M[2][1] = m, M[2][2] = s;
}

Matrix3 Matrix3::operator+(const Matrix3& rhs)
{
	return Matrix3(M[0][0] + rhs.M[0][0], M[0][1] + rhs.M[0][1], M[0][2] + rhs.M[0][2],
		M[1][0] + rhs.M[1][0], M[1][1] + rhs.M[1][1], M[1][2] + rhs.M[1][2],
		M[2][0] + rhs.M[2][0], M[2][1] + rhs.M[2][1], M[2][2] + rhs.M[2][2]);
}

Matrix3 Matrix3::operator-(const Matrix3& rhs)
{
	return Matrix3(M[0][0] - rhs.M[0][0], M[0][1] - rhs.M[0][1],
		M[0][2] - rhs.M[0][2], M[1][0] - rhs.M[1][0], M[1][1] - rhs.M[1][1], 
		M[1][2] - rhs.M[1][2], M[2][0] - rhs.M[2][0], M[2][1] - rhs.M[2][1],
		M[2][2] - rhs.M[2][2]);
}

Matrix3 Matrix3::operator+=(const Matrix3& rhs)
{
	M[0][0] += rhs.M[0][0];
	M[0][1] += rhs.M[0][1];
	M[0][2] += rhs.M[0][2];
	M[1][0] += rhs.M[1][0];
	M[1][1] += rhs.M[1][1];
	M[1][2] += rhs.M[1][2];
	M[2][0] += rhs.M[2][0];
	M[2][1] += rhs.M[2][1];
	M[2][2] += rhs.M[2][2];
	return *this;
}

Matrix3 Matrix3::operator-=(const Matrix3& rhs)
{
	M[0][0] -= rhs.M[0][0];
	M[0][1] -= rhs.M[0][1];
	M[0][2] -= rhs.M[0][2];
	M[1][0] -= rhs.M[1][0];
	M[1][1] -= rhs.M[1][1];
	M[1][2] -= rhs.M[1][2];
	M[2][0] -= rhs.M[2][0];
	M[2][1] -= rhs.M[2][1];
	M[2][2] -= rhs.M[2][2];
	return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& other)
{
	Matrix3 m;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m.M[i][j] = M[i][0] * other.M[0][j] + M[i][1] * other.M[1][j] + M[i][2] * other.M[2][j];
		}
	}

	*this = m;
	return *this;
}

Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
	Matrix3 m(M[0][0] * rhs.M[0][0] + M[0][1] * rhs.M[1][0] + M[0][2] * rhs.M[2][0],
		M[0][0] * rhs.M[0][1] + M[0][1] * rhs.M[1][1] + M[0][2] * rhs.M[2][1],
		M[0][0] * rhs.M[0][2] + M[0][1] * rhs.M[1][2] + M[0][2] * rhs.M[2][2],
		M[1][0] * rhs.M[0][0] + M[1][1] * rhs.M[1][0] + M[1][2] * rhs.M[2][0],
		M[1][0] * rhs.M[0][1] + M[1][1] * rhs.M[1][1] + M[1][2] * rhs.M[2][1],
		M[1][0] * rhs.M[0][2] + M[1][1] * rhs.M[1][2] + M[1][2] * rhs.M[2][2],
		M[2][0] * rhs.M[0][0] + M[2][1] * rhs.M[1][0] + M[2][2] * rhs.M[2][0],
		M[2][0] * rhs.M[0][1] + M[2][1] * rhs.M[1][1] + M[2][2] * rhs.M[2][1],
		M[2][0] * rhs.M[0][2] + M[2][1] * rhs.M[1][2] + M[2][2] * rhs.M[2][2]);
	return m;
}

Matrix3 Matrix3::operator*(double rhs) const
{
	return Matrix3(rhs * M[0][0], rhs * M[0][1], rhs * M[0][2],
		rhs * M[1][0], rhs * M[1][1], rhs * M[1][2],
		rhs * M[2][0], rhs * M[2][1], rhs * M[2][2]);
}

Matrix3& Matrix3::operator*=(double rhs)
{
	M[0][0] *= rhs;
	M[0][1] *= rhs;
	M[0][2] *= rhs;
	M[1][0] *= rhs;
	M[1][1] *= rhs;
	M[1][2] *= rhs;
	M[2][0] *= rhs;
	M[2][1] *= rhs;
	M[2][2] *= rhs;
	return *this;
}

void Matrix3::makeIdentity()
{
	M[0][0] = 1.0;	M[0][1] = 0;	M[0][2] = 0;
	M[1][0] = 0;	M[1][1] = 1.0;	M[1][2] = 0;
	M[2][0] = 0;	M[2][1] = 0;	M[2][2] = 1.0;
}

Matrix3 Matrix3::translate(double l, double m)
{
	Matrix3  matrix;
	matrix.M[2][0] = l;
	matrix.M[2][1] = m;
	return matrix;
}

void Matrix3::makeTranslate(double x, double y)
{
	makeIdentity();
	this->M[2][0] = x;
	this->M[2][1] = y;
}

void Matrix3::makeScale(double x, double y)
{
	makeIdentity();
	this->M[0][0] = x;
	this->M[1][1] = y;
}

Matrix3 Matrix3::Scale(double a, double d)
{
	Matrix3 matrix;
	matrix.M[0][0] = a;
	matrix.M[1][1] = d;
	return matrix;
}

Matrix3 Matrix3::Rotate(double angle)
{
	Matrix3  matrix;
	matrix.M[0][0] = cos(angle);
	matrix.M[1][1] = cos(angle );
	matrix.M[2][2] = 1.0;
	matrix.M[1][0] = -sin(angle);
	matrix.M[0][1] = sin(angle );
	return matrix;
}

Matrix3 Matrix3::Shear(double b, double c)
{
	Matrix3 matrix;
	matrix.M[0][0] = 1.0;
	matrix.M[1][1] = 1.0;
	matrix.M[2][2] = 1.0;
	matrix.M[1][0] = c;
	matrix.M[0][1] = b;
	return matrix;
}

Matrix3 Matrix3::Inverse(Matrix3& matrix)
{
	Matrix3 m;

	m.makeIdentity();

	//求逆矩阵行列式的值
	double tmp = matrix.M[0][0] * matrix.M[1][1] * matrix.M[2][2] + matrix.M[0][1] * matrix.M[1][2] * matrix.M[2][0] + matrix.M[0][2] * matrix.M[1][0] * matrix.M[2][1]
		- matrix.M[0][0] * matrix.M[1][2] * matrix.M[2][1] - matrix.M[0][1] * matrix.M[1][0] * matrix.M[2][2] - matrix.M[0][2] * matrix.M[1][1] * matrix.M[2][0];

	if (tmp == 0)
	{
		return m;
	}

	m.M[0][0] = (matrix.M[1][1] * matrix.M[2][2] - matrix.M[1][2] * matrix.M[2][1]) / tmp;
	m.M[1][0] = (-matrix.M[1][0] * matrix.M[2][2] + matrix.M[1][2] * matrix.M[2][0]) / tmp;
	m.M[2][0] = (matrix.M[1][0] * matrix.M[2][1] - matrix.M[1][1] * matrix.M[2][0]) / tmp;
	m.M[0][1] = (-matrix.M[0][1] * matrix.M[2][2] + matrix.M[0][2] * matrix.M[2][1]) / tmp;
	m.M[1][1] = (matrix.M[0][0] * matrix.M[2][2] - matrix.M[0][1] * matrix.M[2][0]) / tmp;
	m.M[2][1] = (-matrix.M[0][0] * matrix.M[1][1] + matrix.M[0][1] * matrix.M[2][0]) / tmp;
	m.M[0][2] = (matrix.M[0][1] * matrix.M[1][2] - matrix.M[0][2] * matrix.M[1][1]) / tmp;
	m.M[1][2] = (-matrix.M[0][0] * matrix.M[1][2] + matrix.M[0][2] * matrix.M[1][0]) / tmp;
	m.M[2][2] = (matrix.M[0][0] * matrix.M[1][1] - matrix.M[0][1] * matrix.M[1][0]) / tmp;

	return m;

}