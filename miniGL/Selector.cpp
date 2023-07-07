#include"Selector.h"
int Selector::selectLayerByPoint(Layer* pLayer, Point2D& pt, bool isMultipleSelect)
{
	return 0;
}
int Selector::selectGeometryByPoint(Geometry* pGeometry, Point2D& pt, bool isMultipleSelect)
{
	return 0;
}
void Selector::selectGeometry(Geometry* pGeometry, bool isMultipleSelect)
{

}
void Selector::clear()
{

}
bool Selector::empty()
{
	return true;
}
std::vector<Geometry*>& Selector::getSelectedGeometries()
{
	std::vector<Geometry*> a;
	return a;
}

int Selector::ptInGeometry(Geometry* pGeometry, Point2D& pt)
{
	return 0;
}
int Selector::ptOnLine(const Point2D& pt, const Point2D& p1, const Point2D& p2)
{
	return 0;
}
int Selector::ptInPolygon(const Point2D& pt, const std::vector<Point2D>& pts)
{
	return 0;
}