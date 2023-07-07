#include <iostream>
#include <windows.h>
#include "Graphic.h"
#include "Draw.h"
#include "Sta.h"
#include <vector>

using namespace std;
//编码函数
int encode(int x, int y, PixelPoint rect[])
{
    int XL = rect[0].x;
    int XR = rect[1].x;
    int YD = rect[1].y;
    int YU = rect[0].y;
    int code = 0;
    if (x >= XL && x <= XR && y >= YD && y <= YU) code = 0;
    if (x < XL && y >= YD && y <= YU) code = 1;
    if (x > XR && y >= YD && y <= YU) code = 2;
    if (x >= XL && x <= XR && y > YU) code = 8;
    if (x >= XL && x <= XR && y < YD) code = 4;

    if (x < XL && y > YU) code = 9;
    if (x > XR && y > YU) code = 10;
    if (x < XL && y < YD) code = 5;
    if (x > XR && y < YD) code = 6;

    return code;
}

void Draw::drawrect(int x1, int y1, int x2, int y2)
{
    //Draw::drawLine(x1, y1, x2, y1, BLUE);
    //Draw::drawLine(x1, y2, x2, y2, BLUE);
    //Draw::drawLine(x1, y1, x1, y2, BLUE);
    //Draw::drawLine(x2, y1, x2, y2, BLUE);
}

void Draw::CohenCut(int x1, int y1, int x2, int y2, int a, int b, int c, int d)
{
    PixelPoint rect[2];
    if (a < c)
    {
        if (b > d)
        {
            rect[0].x = a;
            rect[1].x = c;
            rect[0].y = b;
            rect[1].y = d;
        }
        else
        {
            rect[0].x = a;
            rect[1].x = c;
            rect[0].y = d;
            rect[1].y = b;
        }
    }
    else
    {
        if (b > d)
        {
            rect[1].x = a;
            rect[0].x = c;
            rect[1].y = d;
            rect[0].y = b;
        }
        else
        {
            rect[1].x = a;
            rect[0].x = c;
            rect[1].y = b;
            rect[0].y = d;
        }
    }

    int XL = rect[0].x;
    int XR = rect[1].x;
    int YD = rect[1].y;
    int YU = rect[0].y;
    drawrect(XL, YU, XR, YD);
    int code1 = 0, code2 = 0, code, x = 0, y = 0;
    code1 = encode(x1, y1, rect);//对线段端点进行编码
    code2 = encode(x2, y2, rect);
    while (code1 != 0 || code2 != 0)
    {
        if ((code1 & code2) != 0) return;
        code = code1;
        if (code1 == 0) code = code2;
        if ((1 & code) != 0)//求线段与窗口左边的交点: 001=1
        {
            x = XL;
            y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
        else if ((2 & code) != 0)
        {
            x = XR;
            y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
        else if ((4 & code) != 0)
        {
            y = YD;
            x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        }
        else if ((8 & code) != 0)
        {
            y = YU;
            x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        }
        if (code == code1)
        {
            x1 = x; y1 = y; code1 = encode(x, y, rect);
        }
        else
        {
            x2 = x; y2 = y; code2 = encode(x, y, rect);
        }
    }
    Draw::drawLine(x1, y1, x2, y2, getPenColor());
}