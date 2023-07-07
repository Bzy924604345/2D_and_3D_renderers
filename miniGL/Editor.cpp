#include "Editor.h"
#include "GeoDefine.h"
#include "Vec3.h"
#include "Draw.h"
#include "Graphic.h"

void Editor::apply(Dataset* pDataset, Matrix3& m)
{
	for (int i = pDataset->getLayerCount() - 1; i >= 0; --i)
	{
		Layer* pLayer = (*pDataset)[i];
		apply(pLayer, m);
	}
}

void Editor::apply(Layer* pLayer, Matrix3& m)
{
	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		apply((*pLayer)[i], m);
	}
}

void Editor::apply(Geometry* pGeometryDef, Matrix3& m)
{
	switch (pGeometryDef->getGeomType())
	{
	case gtPoint:
	{
		PointGeometry* pGeometry = (PointGeometry*)pGeometryDef;
		Vec3 v(pGeometry->x, pGeometry->y);
		Vec3 v1 = v * m;//�任
		pGeometry->x = v1.x();
		pGeometry->y = v1.y();
	}
	break;
	case gtPolyline:
	case gtPolygon:
	{
		PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;//ת��Ϊʵ��ͼ������
		vector<Point2D>& pts = pGeometry->getPts();//���ͼ�ζ��㼯��
		size_t ptsCount = pts.size();
		for (int i = 0; i < ptsCount; ++i)
		{
			Vec3 v(pts[i].x, pts[i].y);
			Vec3 v1 = v * m;//�任
			pts[i].x = v1.x();
			pts[i].y = v1.y();
		}
	}
	break;
	case gtCircle:
	{
		CircleGeometry* pGeometry = (CircleGeometry*)pGeometryDef;
		Vec3 v(pGeometry->x, pGeometry->y);//��ԭʼ���깹���������
		Vec3 v2 = v * m;//�任
		setPixel(v2.x(), v2.y(), getPenColor());//����
		Draw::drawCircle(v2.x(), v2.y(), pGeometry->r, getPenColor());
	}
	break;
	case gtEllipse:
	{
		EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryDef;
		double x, y;
		pGeometry->getCenter(x, y);
		Vec3 v(x, y);//��ԭʼ���깹���������
		Vec3 v2 = v * m;//�任
		setPixel(v2.x(), v2.y(), getPenColor());//����
		//Raster::drawEllipse(v2.x(), v2.y(), pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
	}
	break;
	}
}


