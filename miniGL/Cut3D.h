#pragma once
#include "Graphic.h"
#include "Vec3.h"
#include "GeoDefine.h"
#include "Vec4.h"

struct return_Vec4
{
	Vec4 p1;
	Vec4 p2;
};

class Cut3D
{
	public:
		// ÈýÎ¬Ïß²Ã¼ô
		static return_Vec4 Cut3D::CohenCut3D(double x1, double y1, double z1, double h1, double x2, double y2, double z2, double h2);
};