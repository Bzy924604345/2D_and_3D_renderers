#include "Matrix4.h"
#include"Vec3.h"


Matrix4::Matrix4() {
    _mat[0][0] = 1;_mat[0][1] = 0;_mat[0][2] = 0;_mat[0][3] = 0;
    _mat[1][0] = 0;_mat[1][1] = 1;_mat[1][2] = 0;_mat[1][3] = 0;
    _mat[2][0] = 0;_mat[2][1] = 0;_mat[2][2] = 1;_mat[2][3] = 0;
    _mat[3][0] = 0;_mat[3][1] = 0;_mat[3][2] = 0;_mat[3][3] = 1;
}

Matrix4 Matrix4::makeIdentity() {
    Matrix4 m;
    m._mat[0][0] = 1;
    m._mat[1][1] = 1;
    m._mat[2][2] = 1;
    m._mat[3][3] = 1;
    *this = m;
    return m;
}

Matrix4& Matrix4::operator *=(const Matrix4& other) {
    Matrix4 m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m._mat[i][j] = _mat[i][0] * other._mat[0][j] + _mat[i][1] * other._mat[1][j] + _mat[i][2] * other._mat[2][j] + _mat[i][3] * other._mat[3][j];
        }
    }
    *this = m;
    return *this;
}

// 透视投影矩阵
void Matrix4::makePerspective(double fovy, double aspectRatio, double n, double f) {
    fovy *= PI;
    fovy /= 180;
    _mat[0][0] = 1 / (aspectRatio * tan(fovy/2));
    _mat[1][1] = 1 / tan(fovy/2);
    _mat[2][2] = f / (n - f);
    _mat[2][3] = -1;
    _mat[3][2] = f * n / (n - f);
    _mat[3][3] = 0;
}


// 正交投影矩阵
void Matrix4::makeOrtho(double w, double h, double n, double f) {
    _mat[0][0] = 2 / w;
    _mat[1][1] = 2 / h;
    _mat[2][2] = 1 / (n - f);
    _mat[3][2] = n / (n - f);
}



// 平移变换及矩阵
Matrix4 Matrix4::makeTranslate(double x, double y, double z) {
    Matrix4 m;
    m._mat[3][0] = x;
    m._mat[3][1] = y;
    m._mat[3][2] = z;
    *this = m;
    return m;
}
Matrix4 Matrix4::translate(double dx, double dy, double dz) {
    Matrix4 m;
    m.makeTranslate(dx, dy, dz);
    return m;
}

// 缩放变换及矩阵
Matrix4 Matrix4::makeScale(double x, double y, double z) {
    Matrix4 m;
    m._mat[0][0] = x;
    m._mat[1][1] = y;
    m._mat[2][2] = z;
    *this = m;
    return m;
}
Matrix4 Matrix4::scale(double dx, double dy, double dz) {
    Matrix4 m;
    m.makeScale(dx, dy, dz);
    return m;
}

// 观察变换矩阵
Matrix4  Matrix4::makeLookAt(Vec3 eye, Vec3 target, Vec3 up) {
    Matrix4 m;
    Vec3 Z = eye - target;
    Vec3 X = up ^ Z;
    Vec3 Y = Z ^ X;
    Vec3 x = X.normalize();
    Vec3 y= Y.normalize();
    Vec3 z = Z.normalize();
    m._mat[0][0] = x.x(); m._mat[0][1] = y.x(); m._mat[0][2] = z.x();
    m._mat[1][0] = x.y(); m._mat[1][1] = y.y(); m._mat[1][2] = z.y();
    m._mat[2][0] = x.z();m._mat[2][1] = y.z();m._mat[2][2] = z.z();
    m._mat[3][0] = -(eye * x);m._mat[3][1] = -(eye * y);m._mat[3][2] = -(eye * z);
    *this = m;
    return m;
}


//旋转变换，angle为弧度制
Matrix4 Matrix4::makeRotateX(double angle)
{
    Matrix4 m;
    m._mat[1][1] = cos(angle);
    m._mat[2][2] = cos(angle);
    m._mat[1][2] = sin(angle);
    m._mat[2][1] = -sin(angle);
    *this = m;
    return m;
}

Matrix4 Matrix4::makeRotateY(double angle)
{
    Matrix4 m;
    m._mat[0][0] = cos(angle);
    m._mat[2][2] = cos(angle);
    m._mat[2][0] = sin(angle);
    m._mat[0][2] = -sin(angle);
    *this = m;
    return m;
}

Matrix4 Matrix4::makeRotateZ(double  angle)
{
    Matrix4 m;
    m._mat[0][0] = cos(angle);
    m._mat[1][1] = cos(angle);
    m._mat[0][1] = sin(angle);
    m._mat[1][0] = -sin(angle);
    *this = m;
    return m;
}

//错切变换
Matrix4 Matrix4::makeShareX(double d, double g)
{
    Matrix4 m;
    m._mat[1][0] = d;
    m._mat[2][0] = g;
    *this = m;
    return m;
}

Matrix4 Matrix4::makeShareY(double b, double h)
{
    Matrix4 m;
    m._mat[0][1] = b;
    m._mat[2][1] = h;
    *this = m;
    return m;
}

Matrix4 Matrix4:: operator *(Matrix4 m)const
{
    Matrix4 m1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += this->_mat[i][k] * m._mat[k][j];
            }
            m1._mat[i][j] = sum;
        }
    }
    return m1;
}

//矩阵逆矩阵

Matrix4 Matrix4::Inverse(Matrix4& matrix)
{
    Matrix4 m;

    m.makeIdentity();

    //求逆矩阵行列式的值
    double tmp = matrix._mat[0][0] * matrix._mat[1][1] * matrix._mat[2][2] * matrix._mat[3][3]
        + matrix._mat[0][1] * matrix._mat[1][2] * matrix._mat[2][3] * matrix._mat[3][0]
        + matrix._mat[0][2] * matrix._mat[1][3] * matrix._mat[2][0] * matrix._mat[3][1]
        + matrix._mat[0][3] * matrix._mat[1][0] * matrix._mat[2][1] * matrix._mat[3][2]
        - matrix._mat[0][0] * matrix._mat[1][3] * matrix._mat[2][2] * matrix._mat[3][1]
        - matrix._mat[0][1] * matrix._mat[1][0] * matrix._mat[2][3] * matrix._mat[3][2]
        - matrix._mat[0][2] * matrix._mat[1][1] * matrix._mat[2][0] * matrix._mat[3][3]
        - matrix._mat[0][3] * matrix._mat[1][2] * matrix._mat[2][1] * matrix._mat[3][0];

    if (tmp == 0)
    {
        return m;
    }
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++)
        {
            //构建删除后矩阵
            Matrix3 m1;
            int k = 0;
            int h = 0;
            for (int a = 0; a < 3; a++)
            {
                if (k == i) k++;
                for (int c = 0; c < 3; c++)
                {
                    if (h == j) h++;
                    m1.M[a][c] = matrix._mat[k][h];
                    h++;
                }
                h = 0;
                k++;
            }

            // 计算行列式的值
            double tmp1 = m1.M[0][0] * m1.M[1][1] * m1.M[2][2] + m1.M[0][1] * m1.M[1][2] * m1.M[2][0] + m1.M[0][2] * m1.M[1][0] * m1.M[2][1]
                - m1.M[0][0] * m1.M[1][2] * m1.M[2][1] - m1.M[0][1] * m1.M[1][0] * m1.M[2][2] - m1.M[0][2] * m1.M[1][1] * m1.M[2][0];

            m._mat[i][j] = pow(-1, i + j) * tmp1 / tmp;


             
        }
    }
       

    //m._mat[0][0] = (matrix._mat[1][1] * matrix._mat[2][2] * matrix._mat[3][3] 
    //    + matrix._mat[1][2] * matrix._mat[2][3] * matrix._mat[3][1] / tmp;
    //m._mat[1][0] = (-matrix.M[1][0] * matrix.M[2][2] + matrix.M[1][2] * matrix.M[2][0]) / tmp;
    //m._mat[2][0] = (matrix.M[1][0] * matrix.M[2][1] - matrix.M[1][1] * matrix.M[2][0]) / tmp;
 
    //m._mat[0][1] = (-matrix.M[0][1] * matrix.M[2][2] + matrix.M[0][2] * matrix.M[2][1]) / tmp;
    //m._mat[1][1] = (matrix.M[0][0] * matrix.M[2][2] - matrix.M[0][1] * matrix.M[2][0]) / tmp;
    //m._mat[2][1] = (-matrix.M[0][0] * matrix.M[1][1] + matrix.M[0][1] * matrix.M[2][0]) / tmp;
    //m._mat[0][2] = (matrix.M[0][1] * matrix.M[1][2] - matrix.M[0][2] * matrix.M[1][1]) / tmp;
    //m._mat[1][2] = (-matrix.M[0][0] * matrix.M[1][2] + matrix.M[0][2] * matrix.M[1][0]) / tmp;
    //m._mat[2][2] = (matrix.M[0][0] * matrix.M[1][1] - matrix.M[0][1] * matrix.M[1][0]) / tmp;


    return m;

}

//	calculate transpose of matrix//计算矩阵的转置
Matrix4 Matrix4::transpose(Matrix4 matrix) {
    Matrix4 m;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m._mat[i][j] = matrix._mat[j][i];
    return m;

}