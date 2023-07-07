#pragma once
#include <vector>
#include "GeoDefine.h"

class Selector
{
public:
	Selector() { mTolerance = 3; }
	int selectLayerByPoint(Layer* pLayer, Point2D& pt, bool isMultipleSelect);
	int selectGeometryByPoint(Geometry* pGeometry, Point2D& pt, bool isMultipleSelect);
	void selectGeometry(Geometry* pGeometry, bool isMultipleSelect);
	void clear();
	bool empty();
	std::vector<Geometry*>& getSelectedGeometries();

	int ptInGeometry(Geometry* pGeometry, Point2D& pt);
	int ptOnLine(const Point2D& pt, const Point2D& p1, const Point2D& p2);
	int ptInPolygon(const Point2D& pt, const std::vector<Point2D>& pts);

	std::vector<Geometry*> mSelectedGeometries;
	double mTolerance;
};

