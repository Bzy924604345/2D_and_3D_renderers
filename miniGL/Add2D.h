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

	//�������ݼ�������ͼ��
	virtual void render(Dataset* pDatase);

	//����ͼ��
	virtual void renderLayer(Layer* pLayer);

	//��ԭ
	virtual void reset();

	void applyViewMatrix(Matrix3& m);
	void apply(Matrix3& m);



	//����ͼ�ζ���
	virtual void renderGeometry(Geometry* pGeometryDef, Color color);
	virtual unsigned getLayerColor(Layer* pLayer);

	void init(double xOrig_map, double yOrig_map,
		double width_map, double height_map, 
		double x_wnd, double y_wnd, 
		int width_wnd, int height_wnd);

	void mapreset();

	Dataset* mpDataset;//���ݼ�
	bool mInited; //�Ƿ��ʼ��

	Matrix3 mTransformMatrix;//�任����

	double _calcScale();


	int mWndWidth, mWndHeight;//���ڿ��
	double mXOrig_map, mYOrig_map;//��Ӧ����ԭ��ĵ�ͼ��
	double mMapWidth, mMapHeight;//��ͼ���

	Color mSelectColor;//ѡ����Ⱦ��ɫ
	Matrix3 m;
};

