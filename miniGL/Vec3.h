#pragma once
#include "Graphic.h"
#include <cmath>
#include "Matrix3.h"
#include <vector>
//using namespace std;

//三维向量
class Vec3
{
public:

	double v[3];

	//v[0]是x，v[1]是y,v[2]是比例系数
	Vec3();
	Vec3(double x, double y);
	Vec3(double x, double y, double z);

	//设置各成员函数的值
	void set(double x, double y, double z);

	double Length();

	//操作符重载
	double& operator[](int i) { return v[i]; }
	double& x() { return v[0]; }
	double& y() { return v[1]; }
	double& z() { return v[2]; }

	//操作符重载
	Vec3 operator+(const Vec3& rhs);
	Vec3 operator-(const Vec3& rhs);
	Vec3 operator-=(const Vec3& rhs);
	Vec3 operator+=(const Vec3& rhs);

	//矩阵和向量相乘
	Vec3 operator *(Matrix3& m);
	Vec3& operator *=(Matrix3& m);

	//向量乘向量
	double operator *(Vec3& m);

	//叉积
	Vec3 operator ^ (const Vec3& rhs)const;

	//取反
	Vec3 operator - ();

	//数乘
	Vec3 operator *(double rhs)const;
	Vec3 operator/(double rhs) const;
	Vec3 operator +(double rhs)const;
	Vec3 operator -(double rhs)const;
	Vec3& operator *=(double rhs);
	Vec3& operator /=(double rhs);

	//距离计算
	static double Distance(Vec3& lhs, Vec3& rhs);

	//向量归一化
	Vec3 normalize();
};