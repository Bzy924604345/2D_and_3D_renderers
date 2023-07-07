#pragma once
#include "Graphic.h"
#include "Vec3.h"
#include "GeoDefine.h"
#include "Vec4.h"

class Draw
{
public:
	// ≤√ºÙµ„
	static void PointCut(Vec3 v, int x1, int y1, int x2, int y2);
	// ª≠÷±œﬂ
	static void drawLine(int x0, int y0, int x1, int y1, unsigned color);
	// ª≠’€œﬂ
	static void drawPolyline(PixelPoint polypoint[], int size, unsigned color);
	// ª≠–Èœﬂ
	static void drawDottedLine(int x0, int y0, int x1, int y1, unsigned color);
	// ª≠≤√ºÙ∑∂Œß
	static void drawrect(int x1, int y1, int x2, int y2);
	// œﬂ≤√ºÙ
	static void CohenCut(int x1, int y1, int x2, int y2, int a, int b, int c, int d);
	// √Ê≤√ºÙ
	static void PolygonCutSH(PixelPoint polypoint[], int size, int x1, int y1, int x2, int y2);
	// «ÛΩªµ„
	static PixelPoint jiaodian(PixelPoint firstpoint, PixelPoint secondpoint, PixelPoint s1, PixelPoint s2);


	static void drawPoint(int x, int y, unsigned color);
	//static void drawPolygonOutline(PixelPoint pts[], int size, unsigned color);
	static void PolyScan(PixelPoint polypoint[], int size, unsigned color);
	static void drawCircle(double x0, double y0, double r, unsigned color);
	static void drawEllipse(double x0, double y0, double width, double height, unsigned color);
	static void drawCell(int row, int col, unsigned color);
	static void drawGrid();

	static void Fill4(int x, int y, unsigned bcolor, unsigned fcolor);
	static bool Draw::cull(Vec4 p1, Vec4 p2, Vec4 p3);

	static void Setbottriangle3d(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int r, int g, int b, Vec3 a);
	static void Settoptriangle3d(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int r, int g, int b, Vec3 a);
	static void Setgeneraltriangle3d(point3D A, point3D B, point3D C, int r, int g, int b, Vec3 a);

};

