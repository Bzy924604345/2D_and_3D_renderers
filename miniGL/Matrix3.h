#pragma once
#pragma once
#include"cmath"
#include"Graphic.h"
//#include"GeoDefine.h"
//#define PI 3.1415926535

class  Matrix3 //变换矩阵
{
public:
	//private:
	double M[3][3];

	//————定义矩阵
	Matrix3();
	//————两种构造
	Matrix3(double a, double b, double p, double c, double d, double q, double l, double m);
	Matrix3(double a, double b, double p, double c, double d, double q, double l, double m, double s);

	//————提取该矩阵的值
	double& a() { return M[0][0]; }
	double& b() { return M[0][1]; }
	double& c() { return M[1][0]; }
	double& d() { return M[1][1]; }
	double& p() { return M[0][2]; }
	double& q() { return M[1][2]; }
	double& l() { return M[2][0]; }
	double& m() { return M[2][1]; }
	double& s() { return M[2][2]; }

	//矩阵相加
	Matrix3 operator+(const Matrix3& rhs);

	//相减
	Matrix3 operator-(const Matrix3& rhs);

	//+=运算
	Matrix3 operator+=(const Matrix3& rhs);

	//-=运算
	Matrix3 operator-=(const Matrix3& rhs);

	Matrix3& operator *=(const Matrix3& rhs);

	//矩阵和矩阵之间叉乘
	Matrix3 operator*(const Matrix3& rhs)const;

	//数乘
	Matrix3 operator *(double rhs)const;
	Matrix3& operator *=(double rhs);

	//初始化一个单位阵
	void makeIdentity();

	//平移变换
	static Matrix3 translate(double l, double m);
	void makeTranslate(double x, double y);

	//放缩变换
	void makeScale(double x, double y);
    static Matrix3 Scale(double a, double d);

	//旋转变换
	static Matrix3 Rotate(double angle);

	//错切变换
	static Matrix3 Shear(double b, double c);

	//逆矩阵
	static Matrix3 Inverse(Matrix3& m);
};