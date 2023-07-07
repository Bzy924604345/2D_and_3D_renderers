#pragma once
#include<cmath>
#include"Matrix3.h"
#include"Matrix4.h"
#include <vector>

using namespace std;

//��ά����
class Vec4
{
public:

	double v[4];
	//v[0]��x��v[1]��y,v[2]��z,v[3]�Ǳ���ϵ��
	Vec4();
	Vec4(double x, double y, double z);
	Vec4(double x, double y, double z, double h);
	
	// ��άת��ά
	Vec4(Vec3& v);

	//���ø���Ա������ֵ
	void set(double x, double y, double z, double h);
	void set(double x, double y, double z);

	//����������
	double& operator[](int i) { return v[i]; }
	double& x() { return v[0]; }
	double& y() { return v[1]; }
	double& z() { return v[2]; }
	double& h() { return v[3]; }

	//����������
	bool operator==(const Vec4& rhs);
	Vec4 operator+(const Vec4& rhs);
	Vec4 operator-(const Vec4& rhs);
	Vec4 operator-=(const Vec4& rhs);
	Vec4 operator+=(const Vec4& rhs);

	//������������
	Vec4 operator * (Matrix4& m);

	//���
	Vec4 operator ^ (const Vec4& rhs)const;

	//ȡ��
	Vec4 operator - ();
	
	//����
	Vec4 operator *(double rhs)const;
	Vec4& operator *=(double rhs);

	//����
	Vec4& operator /=(double rhs);

	//������һ��
	Vec4 normalize();
};
