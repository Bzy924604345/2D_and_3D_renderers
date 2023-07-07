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
		Vec3 v(pGeometry->x, pGeometry->y);//由原始坐标构建齐次坐标
		v *= mTransformMatrix;//变换
		setPixel(v.x(), v.y(), color);//绘制
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
			Point2D& pt = (*pGeometry)[j];//获得线顶点的原始坐标
			//setPixel( pt.x, pt.y, color );//绘制

			Vec3 v(pt.x, pt.y);//由原始坐标构建齐次坐标
			v *= m;//变换
			//setPixel(v2.x(), v2.y(), color);//绘制
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
		//PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;//转化为实际图形类型
		//const vector<Point2D>& pts = pGeometry->getPts();//获得图形顶点集合
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
		PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryDef; //转化为实际图形类型
		const vector<Point2D>& pts = pGeometry->getPts(); ////获得图形顶点集合

		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());

		//	size_t ptsCount = pts.size();
	//	vector <PixelPoint> _pts(ptsCount);//drawPolygon函数参数是PixelPoint数组，_pts用于接收Point2D转化的PixelPoint数组
	//	for (int i = 0; i < ptsCount; ++i)
	//	{
	//		Vec3 v(pts[i].x, pts[i].y);
	//		Vec3 v1 = v * mTransformMatrix;
	//		_pts[i].x = v1.x();
	//		_pts[i].y = v1.y();
		
		for (int j = 0, ptsCount = pGeometry->getPts().size(); j < ptsCount; ++j)
		{
			Point2D& pt = (*pGeometry)[j];//获得多边形顶点的原始坐标

			Vec3 v(pt.x, pt.y);//由原始坐标构建齐次坐标
			v *= mTransformMatrix;//变换
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

//应用视图矩阵
void Renderer2D::applyViewMatrix(Matrix3& m)
{
	this->m *= m;
}
