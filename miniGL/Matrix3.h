#pragma once
#pragma once
#include"cmath"
#include"Graphic.h"
//#include"GeoDefine.h"
//#define PI 3.1415926535

class  Matrix3 //�任����
{
public:
	//private:
	double M[3][3];

	//���������������
	Matrix3();
	//�����������ֹ���
	Matrix3(double a, double b, double p, double c, double d, double q, double l, double m);
	Matrix3(double a, double b, double p, double c, double d, double q, double l, double m, double s);

	//����������ȡ�þ����ֵ
	double& a() { return M[0][0]; }
	double& b() { return M[0][1]; }
	double& c() { return M[1][0]; }
	double& d() { return M[1][1]; }
	double& p() { return M[0][2]; }
	double& q() { return M[1][2]; }
	double& l() { return M[2][0]; }
	double& m() { return M[2][1]; }
	double& s() { return M[2][2]; }

	//�������
	Matrix3 operator+(const Matrix3& rhs);

	//���
	Matrix3 operator-(const Matrix3& rhs);

	//+=����
	Matrix3 operator+=(const Matrix3& rhs);

	//-=����
	Matrix3 operator-=(const Matrix3& rhs);

	Matrix3& operator *=(const Matrix3& rhs);

	//����;���֮����
	Matrix3 operator*(const Matrix3& rhs)const;

	//����
	Matrix3 operator *(double rhs)const;
	Matrix3& operator *=(double rhs);

	//��ʼ��һ����λ��
	void makeIdentity();

	//ƽ�Ʊ任
	static Matrix3 translate(double l, double m);
	void makeTranslate(double x, double y);

	//�����任
	void makeScale(double x, double y);
    static Matrix3 Scale(double a, double d);

	//��ת�任
	static Matrix3 Rotate(double angle);

	//���б任
	static Matrix3 Shear(double b, double c);

	//�����
	static Matrix3 Inverse(Matrix3& m);
};