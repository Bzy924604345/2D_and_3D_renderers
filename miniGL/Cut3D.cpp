#include <iostream>
#include <windows.h>
#include "Graphic.h"
#include "Cut3D.h"
#include "Sta.h"
#include <vector>

using namespace std;
//编码函数
int encode3D(int x, int y, int z,int w)
{
    int code = 0;
    if (x >= 0 && x > w && y >= 0 && y > w && z >= 0 && z > w) code = 42;
    if (x >= 0 && x <= w && y >= 0 && y > w && z >= 0 && z > w) code = 40;
    if (x < 0 && x <= w && y >= 0 && y > w && z >= 0 && z > w) code = 41;
    
    if (x >= 0 && x > w && y < 0 && y > w && z >= 0 && z > w) code = 34;
    if (x >= 0 && x <= w && y < 0 && y > w && z >= 0 && z > w) code = 32;
    if (x < 0 && x <= w && y < 0 && y > w && z >= 0 && z > w) code = 33;

    if (x >= 0 && x > w && y < 0 && y <= w && z >= 0 && z > w) code = 38;
    if (x >= 0 && x <= w && y < 0 && y <= w && z >= 0 && z > w) code = 36;
    if (x < 0 && x <= w && y < 0 && y <= w && z >= 0 && z > w) code = 37;

    if (x >= 0 && x > w && y >= 0 && y > w && z >= 0 && z <= w) code = 10;
    if (x >= 0 && x <= w && y >= 0 && y > w && z >= 0 && z <= w) code = 8;
    if (x < 0 && x <= w && y >= 0 && y > w && z >= 0 && z <= w) code = 9;

    if (x >= 0 && x > w && y >= 0 && y <= w && z >= 0 && z <= w) code = 2;
    if (x >= 0 && x <= w && y >= 0 && y <= w && z >= 0 && z <= w) code = 0;
    if (x < 0 && x <= w && y >= 0 && y <= w && z >= 0 && z <= w) code = 1;

    if (x >= 0 && x > w && y < 0 && y <= w && z >= 0 && z <= w) code = 6;
    if (x >= 0 && x <= w && y < 0 && y <= w && z >= 0 && z <= w) code = 4;
    if (x < 0 && x <= w && y < 0 && y <= w && z >= 0 && z <= w) code = 5;

    if (x >= 0 && x > w && y >= 0 && y > w && z < 0 && z <= w) code = 26;
    if (x >= 0 && x <= w && y >= 0 && y > w && z < 0 && z <= w) code = 24;
    if (x < 0 && x <= w && y >= 0 && y > w && z < 0 && z <= w) code = 25;

    if (x >= 0 && x > w && y >= 0 && y <= w && z < 0 && z <= w) code = 18;
    if (x >= 0 && x <= w && y >= 0 && y <= w && z < 0 && z <= w) code = 16;
    if (x < 0 && x <= w && y >= 0 && y <= w && z < 0 && z <= w) code = 17;

    if (x >= -w && x > w && y < 0 && y <= w && z < 0 && z <= w) code = 22;
    if (x >= -w && x <= w && y < 0 && y <= w && z < 0 && z <= w) code = 20;
    if (x < -w && x <= w && y < 0 && y <= w && z < 0 && z <= w) code = 21;

    return code;
}

return_Vec4 Cut3D::CohenCut3D(double x1, double y1, double z1, double h1, double x2, double y2, double z2, double h2)
{
    return_Vec4 return_V ;
    Vec4 v1(0,0,0,0);
    Vec4 v2(0,0,0,0);
    return_V.p1 = v1;
    return_V.p2 = v2;
    int code1 = 0, code2 = 0;
    double t =1;
    code1 = encode3D(x1, y1, z1, h1);//对线段端点进行编码
    code2 = encode3D(x2, y2, z2, h2);

    while (code1 != 0 || code2 != 0) // 非简取
    {
        if ((code1 & code2) != 0) // 简弃
            return return_V;

        if ((1 & code1) != 0) //求端点一与左平面的交点
        {
            t = x1 / (x1 - x2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }
        else if ((2 & code1) != 0) //右平面
        {
            t = (x1 - h1) / (x1 - x2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }
        else if ((4 & code1) != 0)  //下平面
        {
            t = y1 / (y1 - y2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }
        else if ((8 & code1) != 0) //上平面
        {
            t = (y1 - h1) / (y1 - y2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }
        else if ((16 & code1) != 0) // 近平面
        {
            t = (z1) / (z1 - z2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }
        else if ((32 & code1) != 0) // 远平面
        {
            t = (z1- h1) / (z1 - z2);
            x1 = x1 + (x2 - x1) * t;
            y1 = y1 + (y2 - y1) * t;
            z1 = z1 + (z2 - z1) * t;
            h1 = h1 + (h2 - h1) * t;
        }

        code1 = encode3D(x1, y1, z1, h1);

        if ((1 & code2) != 0) //求端点二与左平面的交点
        {
            t = x1 / (x1 - x2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }
        else if ((2 & code2) != 0) //右平面
        {
            t = (x1 - h2) / (x1 - x2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }
        else if ((4 & code2) != 0)  //下平面
        {
            t = (y1) / (y1 - y2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }
        else if ((8 & code2) != 0) //上平面
        {
            t = (y1 - h2) / (y1 - y2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }
        else if ((16 & code2) != 0) // 近平面
        {
            t = (z1) / (z1 - z2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }
        else if ((32 & code2) != 0) // 远平面
        {
            t = (z1 - h2) / (z1 - z2);
            x2 = x1 + (x2 - x1) * t;
            y2 = y1 + (y2 - y1) * t;
            z2 = z1 + (z2 - z1) * t;
            h2 = h1 + (h2 - h1) * t;
        }

        code2 = encode3D(x2, y2, z2, h2);
    }

    v1.set(x1, y1, z1, h1);
    v2.set(x2, y2, z2, h2);
    return_V.p1 = v1;
    return_V.p2 = v2;

    return return_V;
}