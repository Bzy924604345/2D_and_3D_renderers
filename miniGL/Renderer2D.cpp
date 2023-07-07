#include "Renderer2D.h"
#include "GeoDefine.h"
#include "Selector.h"
#include "Graphic.h"
#include "Vec3.h"
#include "Sta.h"
#include "Cut.h"

Renderer2D::Renderer2D()
{}

void Renderer2D::render(Dataset* pDataset)
{

	for (int i = pDataset->getLayerCount() - 1; i >= 0; --i)
	{

		Layer* pLayer = (*pDataset)[i];
		renderLayer(pLayer);
	}
}

void Renderer2D::renderLayer(Layer* pLayer)
{
	unsigned color = getLayerColor(pLayer);

	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		renderGeometry((*pLayer)[i], color);
	}
}

void Renderer2D::renderGeometry(Geometry* pGeometryDef, Color color)
{
	switch (pGeometryDef->getGeomType())
	{
	case gtPoint:
	{
		PointGeometry* pGeometry = (PointGeometry*)pGeometryDef;
		Vec3 v(pGeometry->x, pGeometry->y);//��ԭʼ���깹���������
		v *= mTransformMatrix;//�任
		setPixel(v.x(), v.y(), color);//����
	}
	break;
	case gtPolyline:
	{
		PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;
		const vector<Point2D>& pts = pGeometry->getPts();
		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int j = 0, ptsCount = pts.size(); j < ptsCount; ++j)
		{
			Point2D& pt = (*pGeometry)[j];//����߶����ԭʼ����
			//setPixel( pt.x, pt.y, color );//����

			Vec3 v(pt.x, pt.y);//��ԭʼ���깹���������
			v *= m;//�任
			//setPixel(v2.x(), v2.y(), color);//����
			rasterPts[j].x = v.x(), rasterPts[j].y = v.y();
		}
		if (g_Sta.Cut == 1)
		{
			//for (int j = 0; j < g_Sta.side.size() - 1; j++)
			//{
				int c = rasterPts.size();
			//	if (rasterPts[0].x == rasterPts[c - 1].x && rasterPts[0].x == rasterPts[c - 1].x)
			//	 Cut::PolygonCut(rasterPts.data(), rasterPts.size(), g_Sta.side[j].x, g_Sta.side[j].y, g_Sta.side[j + 1].x, g_Sta.side[j + 1].y, 0);
			//	else
			//	{
					for (int i = 0; i < c - 1; ++i)
						Cut::CohenCut(rasterPts[i].x, rasterPts[i].y, rasterPts[(i + 1)].x, rasterPts[(i + 1)].y, g_Sta.side[0].x, g_Sta.side[0].y, g_Sta.side[1].x, g_Sta.side[1].y);
			//	}
			//	j++;
			//}
		}
		//else
		//{
		//	Cut::drawPolyline(rasterPts.data(), rasterPts.size(), color);
		//}
		//PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;//ת��Ϊʵ��ͼ������
		//const vector<Point2D>& pts = pGeometry->getPts();//���ͼ�ζ��㼯��
		//for (int i = 0, ptsCount = pts.size(); i < ptsCount - 1; ++i)
		//{
		//	Vec3 v0(pts[i].x, pts[i].y);
		//	Vec3 v1(pts[i + 1].x, pts[i + 1].y);
		//	v0 *= mTransformMatrix;
		//	v1 *= mTransformMatrix;
		//	drawLine(v0.x(), v0.y(), v1.x(), v1.y());
		//}
	}
	break;


	case gtPolygon:
	{
		PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryDef; //ת��Ϊʵ��ͼ������
		const vector<Point2D>& pts = pGeometry->getPts(); ////���ͼ�ζ��㼯��

		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());

		//	size_t ptsCount = pts.size();
	//	vector <PixelPoint> _pts(ptsCount);//drawPolygon����������PixelPoint���飬_pts���ڽ���Point2Dת����PixelPoint����
	//	for (int i = 0; i < ptsCount; ++i)
	//	{
	//		Vec3 v(pts[i].x, pts[i].y);
	//		Vec3 v1 = v * mTransformMatrix;
	//		_pts[i].x = v1.x();
	//		_pts[i].y = v1.y();
		
		for (int j = 0, ptsCount = pGeometry->getPts().size(); j < ptsCount; ++j)
		{
			Point2D& pt = (*pGeometry)[j];//��ö���ζ����ԭʼ����

			Vec3 v(pt.x, pt.y);//��ԭʼ���깹���������
			v *= mTransformMatrix;//�任
			rasterPts[j].x = v.x(), rasterPts[j].y = v.y();

		}
		//drawPolygon(rasterPts.data(), pts.size());
		if (g_Sta.Cut == 1)
		{
			for (int j = 0; j < g_Sta.side.size(); j++)
			{
				Cut::PolygonCut(rasterPts.data(), rasterPts.size(), g_Sta.side[j].x, g_Sta.side[j].y, g_Sta.side[j + 1].x, g_Sta.side[j + 1].y, 1);
				j++;
			}
		}
		else Cut::PolyScan(rasterPts.data(), rasterPts.size(), YELLOW);
		
	}
	}
}

unsigned Renderer2D::getLayerColor(Layer* pLayer)
{
	return getPenColor();
}

//void Renderer2D::translate(double dx, double dy)
//{
//	m *= Matrix3::translate(dx, dy);
//}

void Renderer2D::reset()
{
	mTransformMatrix.makeIdentity();
}

//Ӧ����ͼ����
void Renderer2D::applyViewMatrix(Matrix3& m)
{
	this->m *= m;
}
