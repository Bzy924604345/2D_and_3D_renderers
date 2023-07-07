#include "Add2D.h"
#include "GeoDefine.h"
#include "Selector.h"
#include "Graphic.h"
#include "Vec3.h"
#include "Sta.h"
#include "Draw.h"

Matrix3 m_layer;


void Add2D::render(Dataset* pDataset)
{
	if (g_Sta.DrawMode == Sta::dmGrid)
	{
		Draw::drawGrid();
	}
		
	for (int i = pDataset->getLayerCount() - 1; i >= 0; --i)
	{
		Layer* pLayer = (*pDataset)[i];
		renderLayer(pLayer);
	}
}

void Add2D::renderLayer(Layer* pLayer)
{
	unsigned color = getLayerColor(pLayer);

	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		renderGeometry((*pLayer)[i], color);
	}
}

void Add2D::renderGeometry(Geometry* pGeometryDef, Color color)
{
	switch (pGeometryDef->getGeomType())
	{
	case gtPoint:
	{
		PointGeometry* pGeometry = (PointGeometry*)pGeometryDef;
		Vec3 v(pGeometry->x, pGeometry->y);//由原始坐标构建齐次坐标
		v *= m;//变换
		v *= m_layer;
		static vector<PixelPoint> rasterPts;
		if (g_Sta.Cut == 1)
		{
			for (int j = 0; j < g_Sta.side.size() - 1; j++)
			{
				Draw::PointCut(v, g_Sta.side[j].x, g_Sta.side[j].y, g_Sta.side[j + 1].x, g_Sta.side[j + 1].y);
				j++;
			}
		}
		else
		{
			setPixel(v.x(), v.y(), getPenColor());//绘制
		}

	}
	break;
	case gtPolyline:
	{
		PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;//转化为实际图形类型
		const vector<Point2D>& pts = pGeometry->getPts();//获得图形顶点集合
		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int j = 0, ptsCount = pts.size(); j < ptsCount; ++j)
		{
			Point2D& pt = (*pGeometry)[j];//获得线顶点的原始坐标
			Vec3 v(pt.x, pt.y);//由原始坐标构建齐次坐标
			v *= m;//变换
			v *= m_layer;
			rasterPts[j].x = v.x(), rasterPts[j].y = v.y();
		}

		if (g_Sta.Cut == 1)
		{
			for (int j = 0; j < g_Sta.side.size() - 1; j++)
			{
				int c = rasterPts.size();
				for (int i = 0; i < c - 1; ++i)
					Draw::CohenCut(rasterPts[i].x, rasterPts[i].y, rasterPts[(i + 1)].x, rasterPts[(i + 1)].y, g_Sta.side[j].x, g_Sta.side[j].y, g_Sta.side[j + 1].x, g_Sta.side[j + 1].y);
				j++;
			}

		}
		else
		{
			Draw::drawPolyline(rasterPts.data(), rasterPts.size(), getPenColor());
			//Draw::drawDottedLine(rasterPts[0].x,rasterPts[0].y, rasterPts[1].x, rasterPts[1].y ,color);
		}

	}
	break;
	case gtPolygon:
	{
		PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryDef;//转化为实际图形类型
		const vector<Point2D>& pts = pGeometry->getPts();//获得图形顶点集合
		size_t ptsCount = pts.size();
		vector <PixelPoint> rasterPts(ptsCount);
		rasterPts.resize(pts.size());//drawPolygon函数参数是PixelPoint数组，rasterPts用于接收Point2D转化的PixelPoint数组

		for (int i = 0; i < ptsCount; ++i)
		{
			Vec3 v(pts[i].x, pts[i].y);//由原始坐标构建齐次坐标
			v *= m;//变换
			rasterPts[i].x = v.x();
			rasterPts[i].y = v.y();
		}

		if (g_Sta.Cut == 1)
		{
			for (int j = 0; j < g_Sta.side.size(); j++)
			{
				Draw::PolygonCutSH(rasterPts.data(), rasterPts.size(), g_Sta.side[j].x, g_Sta.side[j].y, g_Sta.side[j + 1].x, g_Sta.side[j + 1].y);
				j++;
			}
		}
		else
			///*if (g_Sta.DrawMode == Sta::dmGrid)
			//{*/
			Draw::PolyScan(rasterPts.data(), ptsCount, color);//修改，可显示像素
			/////*}
			////else*/
			/*drawPolygon(rasterPts.data(), ptsCount);*/
			
	}
	break;
	case gtCircle:
	{
		CircleGeometry* pGeometry = (CircleGeometry*)pGeometryDef;
		Vec3 v(pGeometry->x, pGeometry->y);//由原始坐标构建齐次坐标
		Vec3 v2 = v * mTransformMatrix;//变换
		setPixel(v2.x(), v2.y(), color);//绘制
		Draw::drawCircle(v2.x(), v2.y(), pGeometry->r, getPenColor());
	}
	break;
	case gtEllipse:
	{
		EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryDef;
		double x, y;
		pGeometry->getCenter(x, y);
		Vec3 v(x, y);//由原始坐标构建齐次坐标
		Vec3 v2 = v * mTransformMatrix;//变换
		Draw::drawEllipse(v2.x(), v2.y(), pGeometry->getWidth(), pGeometry->getHeight(), getPenColor());
	}
	break;

	}
}

unsigned Add2D::getLayerColor(Layer* pLayer)
{
	return getPenColor();
}


void Add2D::reset()
{
	mTransformMatrix.makeIdentity();
}


void Add2D::applyViewMatrix(Matrix3& m)
{
	this->m *= m;
}


void Add2D::init(double x_center_map, double y_center_map,
	double width_map, double height_map, double x_wnd, double y_wnd,
	int width_wnd, int height_wnd)
{
	double s = (std::min)(width_wnd / width_map, height_wnd / height_map);
	double w = width_wnd / s;
	double h = height_wnd / s;
	m.makeTranslate(w / 2 - x_center_map, h / 2 - y_center_map);
	m *= Matrix3::Scale(s, s);
	m_layer *= Matrix3::Scale(1 / s, 1 / s);
	m_layer *= Matrix3::translate(x_center_map - w / 2,  y_center_map - h / 2 );
}