#pragma once

#include "GeoDefine.h"

struct PixelPoint;
class GeometryFactory
{
public:
	static Geometry* createPointGeometry(double x, double y);

	static Geometry* createPolylineGeometry(PixelPoint* pts, int size);
	static Geometry* createPolylineGeometry(Point2D* pts, int size);
	static Geometry* createPolylineGeometry(PixelPoint& pt1, PixelPoint& pt2);

	static Geometry* createPolygonGeometry(PixelPoint* pts, int size);
	static Geometry* createPolygonGeometry(Point2D* pts, int size);

	static Geometry* createPolygonOutlineGeometry(PixelPoint* pts, int size);
	static Geometry* createPolygonOutlineGeometry(Point2D* pts, int size);

	static Geometry* createCircleGeometry(PixelPoint* pts, int size);
	static Geometry* createCircleGeometry(double x, double y, double r);
	static Geometry* createCircleGeometry(double x1, double y1, double x2, double y2);

	static Geometry* createEllipseGeometry(PixelPoint* pts, int size);
	static Geometry* createEllipseGeometry(double x1, double y1, double x2, double y2);
	static Geometry* creatRectangleGeometry(PixelPoint* pts, int size);
	static Geometry* creatRectangleGeometry(Point2D* pts, int size);
	static Geometry* creatRectangleGeometry(double x1, double y1, double x2, double y2);

	static Geometry* creatRectangleOutlineGeometry(PixelPoint* pts, int size);
	static Geometry* creatRectangleOutlineGeometry(Point2D* pts, int size);
	static Geometry* creatRectangleOutlineGeometry(double x1, double y1, double x2, double y2);
};


