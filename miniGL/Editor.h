#pragma once
#include "Matrix3.h"

struct Geometry;
struct Dataset;
struct Layer;
class Editor
{
public:
	//绘制数据集包含的图层
	void apply(Dataset* pDataset, Matrix3& m);

	//绘制图层
	void apply(Layer* pLayer, Matrix3& m);

	//绘制图形对象
	void apply(Geometry* pGeometryDef, Matrix3& m);

	Matrix3 mTransformMatrix;//变换矩阵
};

