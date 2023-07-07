#ifndef _STA_H
#define _STA_H
#include "Graphic.h"
#include <vector>
#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "GeoDefine.h"
#include "GeoDefine3D.h"
#include "GeometryFactory.h"
#include "Vec3.h"
struct Sta
{
	//enum VisualModeType{ vm2DBasic, vm2DMap, vm3DBasic, vm3DMap }
	//VisualModeType VisualMode ; 
	enum DrawModeType { dmPixel, dmGrid };
	///绘制模式
	DrawModeType DrawMode;//绘制模式，0-像素模式，1-网格模式
	int GridWidth;//网格宽
	int GridHeight;//网格高
	bool Cut = 0;//裁剪判断
	vector<PixelPoint> side;//矩形裁剪框
	bool Reset = 0;//重置判断
	bool g_3d_CutWay = 0; //判断边裁剪方式
	/*PixelPoint Seed_Points[100];*/
	unsigned GridColor1;//网格模式下颜色1
	unsigned GridColor2;//网格模式下颜色2
	CullMode CullMode;//剔除模式

	enum DrawLineMethodType { dlDDA, dlMidBresenham };

	///画线方法
	DrawLineMethodType DrawLineMethod;// 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
	///画多边形方法
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- X扫描线 ， 1 - 活性边表 ， 2--边缘填充

	//——————三维变换矩阵与设置
	int draw3D = 0;//0：普通线框 1：消隐着色
	struct MouseStruct3D
	{
		int oldX, oldY, oldZ;
		int newX, newY, newZ;
	}MouseSet3D;
	Sta();
	int Light = false;//判断光照
	double Ka;//环境光系数
	double Kd;//漫反射系数
	double Ks;//镜面系数
};

extern Sta g_Sta;

#endif
