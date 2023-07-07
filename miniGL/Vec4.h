#pragma once
#include<cmath>
#include"Matrix3.h"
#include"Matrix4.h"
#include <vector>

using namespace std;

//四维向量
class Vec4
{
public:

	double v[4];
	//v[0]是x，v[1]是y,v[2]是z,v[3]是比例系数
	Vec4();
	Vec4(double x, double y, double z);
	Vec4(double x, double y, double z, double h);
	
	// 三维转四维
	Vec4(Vec3& v);

	//设置各成员函数的值
	void set(double x, double y, double z, double h);
	void set(double x, double y, double z);

	//操作符重载
	double& operator[](int i) { return v[i]; }
	double& x() { return v[0]; }
	double& y() { return v[1]; }
	double& z() { return v[2]; }
	double& h() { return v[3]; }

	//操作符重载
	bool operator==(const Vec4& rhs);
	Vec4 operator+(const Vec4& rhs);
	Vec4 operator-(const Vec4& rhs);
	Vec4 operator-=(const Vec4& rhs);
	Vec4 operator+=(const Vec4& rhs);

	//矩阵和向量相乘
	Vec4 operator * (Matrix4& m);

	//叉积
	Vec4 operator ^ (const Vec4& rhs)const;

	//取反
	Vec4 operator - ();
	
	//数乘
	Vec4 operator *(double rhs)const;
	Vec4& operator *=(double rhs);

	//数除
	Vec4& operator /=(double rhs);

	//向量归一化
	Vec4 normalize();
};
