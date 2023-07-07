#pragma once

#include "Graphic.h"
#include "Matrix3.h"

struct Geometry;
struct Dataset;
struct Layer;
class Selector;
class Add2D
{
public:
	Add2D(){ mSelectColor = RED; }

	//绘制数据集包含的图层
	virtual void render(Dataset* pDatase);

	//绘制图层
	virtual void renderLayer(Layer* pLayer);

	//复原
	virtual void reset();

	void applyViewMatrix(Matrix3& m);
	void apply(Matrix3& m);



	//绘制图形对象
	virtual void renderGeometry(Geometry* pGeometryDef, Color color);
	virtual unsigned getLayerColor(Layer* pLayer);

	void init(double xOrig_map, double yOrig_map,
		double width_map, double height_map, 
		double x_wnd, double y_wnd, 
		int width_wnd, int height_wnd);

	void mapreset();

	Dataset* mpDataset;//数据集
	bool mInited; //是否初始化

	Matrix3 mTransformMatrix;//变换矩阵

	double _calcScale();


	int mWndWidth, mWndHeight;//窗口宽高
	double mXOrig_map, mYOrig_map;//对应窗口原点的地图点
	double mMapWidth, mMapHeight;//地图宽高

	Color mSelectColor;//选择集渲染颜色
	Matrix3 m;
};

