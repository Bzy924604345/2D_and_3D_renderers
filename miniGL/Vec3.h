#pragma once
#include "Graphic.h"
#include <cmath>
#include "Matrix3.h"
#include <vector>
//using namespace std;

//��ά����
class Vec3
{
public:

	double v[3];

	//v[0]��x��v[1]��y,v[2]�Ǳ���ϵ��
	Vec3();
	Vec3(double x, double y);
	Vec3(double x, double y, double z);

	//���ø���Ա������ֵ
	void set(double x, double y, double z);

	double Length();

	//����������
	double& operator[](int i) { return v[i]; }
	double& x() { return v[0]; }
	double& y() { return v[1]; }
	double& z() { return v[2]; }

	//����������
	Vec3 operator+(const Vec3& rhs);
	Vec3 operator-(const Vec3& rhs);
	Vec3 operator-=(const Vec3& rhs);
	Vec3 operator+=(const Vec3& rhs);

	//������������
	Vec3 operator *(Matrix3& m);
	Vec3& operator *=(Matrix3& m);

	//����������
	double operator *(Vec3& m);

	//���
	Vec3 operator ^ (const Vec3& rhs)const;

	//ȡ��
	Vec3 operator - ();

	//����
	Vec3 operator *(double rhs)const;
	Vec3 operator/(double rhs) const;
	Vec3 operator +(double rhs)const;
	Vec3 operator -(double rhs)const;
	Vec3& operator *=(double rhs);
	Vec3& operator /=(double rhs);

	//�������
	static double Distance(Vec3& lhs, Vec3& rhs);

	//������һ��
	Vec3 normalize();
};