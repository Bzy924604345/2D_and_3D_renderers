#pragma once
#pragma once
#pragma once
#include"cmath"
#include"Graphic.h"
#include"Vec3.h"
#define PI 3.1415926535

class Matrix4 {
public:
    Matrix4();
    Matrix4 makeIdentity();
    Matrix4& operator*=(const Matrix4& other);
    Matrix4 makeScale(double x, double y, double z);
    double _mat[4][4];
    //投影矩阵
    void makePerspective(double fovy, double aspectRatio, double n, double f);
    void makeOrtho(double w, double h, double n, double f);
    Matrix4 makeTranslate(double x, double y, double z);
    static Matrix4 scale(double dx, double dy, double dz);
    static Matrix4 translate(double dx, double dy, double dz);
    Matrix4 makeLookAt(Vec3 eye, Vec3 center, Vec3 up);//2
    double getElement(int a, int b) { return _mat[a][b]; }
    //旋转变换(绕Y轴）angle为弧度制
    Matrix4 makeRotateY(double angle);
    Matrix4 makeRotateX(double angle);
    Matrix4 makeRotateZ(double angle);
    //错切变换
    Matrix4 makeShareX(double d,double g);
    Matrix4 makeShareY(double b,double h);
    Matrix4 operator *(Matrix4 rhs)const;
    //逆矩阵
    static Matrix4 Inverse(Matrix4& matrix);
    static Matrix4 transpose(Matrix4 matrix);

};
