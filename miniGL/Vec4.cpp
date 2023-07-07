#include "Graphic.h"
#include"Matrix3.h"
#include"Matrix4.h"
#include"Vec4.h"


Vec4::Vec4()
{
	v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; v[3] = 1.0;
}

Vec4::Vec4(double x, double y, double z)
{
	v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0;
}

Vec4::Vec4(double x, double y, double z, double h)
{
	v[0] = x; v[1] = y; v[2] = z; v[3] = h;
}

// ��άת��ά
Vec4::Vec4(Vec3& v)
{
	set(v.v[0], v.v[1], v.v[2]);
}


//���ø���Ա������ֵ
void Vec4::set(double x, double y, double z, double h)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = h;
}

void Vec4::set(double x, double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 1.0;
}

//����������
bool Vec4::operator==(const Vec4& rhs)
{
	if (v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2] && v[3] == rhs.v[3])
		return true;
	else
		return false;
}

Vec4 Vec4::operator+(const Vec4& rhs)
{
	return Vec4(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2], v[3] + rhs.v[3]);
}


Vec4 Vec4::operator-(const Vec4& rhs)
{
	return Vec4(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2], v[3] - rhs.v[3]);
}
Vec4 Vec4::operator-=(const Vec4& rhs)
{
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	v[3] -= rhs.v[3];
	return *this;
}
Vec4 Vec4::operator+=(const Vec4& rhs)
{
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	v[3] += rhs.v[3];
	return *this;
}

//������������
Vec4 Vec4::operator * (Matrix4& m)
{
	return Vec4(v[0] * m._mat[0][0] + v[1] * m._mat[1][0] + v[2] * m._mat[2][0] + v[3] * m._mat[3][0],
		v[0] * m._mat[0][1] + v[1] * m._mat[1][1] + v[2] * m._mat[2][1] + v[3] * m._mat[3][1],
		v[0] * m._mat[0][2] + v[1] * m._mat[1][2] + v[2] * m._mat[2][2] + v[3] * m._mat[3][2],
		v[0] * m._mat[0][3] + v[1] * m._mat[1][3] + v[2] * m._mat[2][3] + v[3] * m._mat[3][3]
	);
}

//���
Vec4 Vec4::operator ^ (const Vec4& rhs)const
{
	return Vec4(v[1] * rhs.v[2] - v[2] * rhs.v[1], v[2] * rhs.v[0] - v[0] * rhs.v[2], v[0] * rhs.v[1] - v[1] * rhs.v[0]);
}


//ȡ��
Vec4 Vec4::operator - ()
{
	return Vec4(-v[0], -v[1], -v[2], -v[3]);
}

//����
Vec4 Vec4::operator *(double rhs)const
{
	return Vec4(rhs * v[0], rhs * v[1], rhs * v[2], rhs * v[3]);
}

Vec4& Vec4::operator *=(double rhs)
{
	v[0] *= rhs;
	v[1] *= rhs;
	v[2] *= rhs;
	v[3] *= rhs;
	return *this;
}

//����
Vec4& Vec4::operator /=(double rhs)
{
	v[0] /= rhs;
	v[1] /= rhs;
	v[2] /= rhs;
	v[3] /= rhs;
	return *this;
}

//������һ��
Vec4 Vec4::normalize()
{
	return  Vec4(v[0] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]), v[1] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]), v[2] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]), v[3] / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]));
}




