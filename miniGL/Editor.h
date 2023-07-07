#pragma once
#include "Matrix3.h"

struct Geometry;
struct Dataset;
struct Layer;
class Editor
{
public:
	//�������ݼ�������ͼ��
	void apply(Dataset* pDataset, Matrix3& m);

	//����ͼ��
	void apply(Layer* pLayer, Matrix3& m);

	//����ͼ�ζ���
	void apply(Geometry* pGeometryDef, Matrix3& m);

	Matrix3 mTransformMatrix;//�任����
};

