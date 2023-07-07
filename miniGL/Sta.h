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
	///����ģʽ
	DrawModeType DrawMode;//����ģʽ��0-����ģʽ��1-����ģʽ
	int GridWidth;//�����
	int GridHeight;//�����
	bool Cut = 0;//�ü��ж�
	vector<PixelPoint> side;//���βü���
	bool Reset = 0;//�����ж�
	bool g_3d_CutWay = 0; //�жϱ߲ü���ʽ
	/*PixelPoint Seed_Points[100];*/
	unsigned GridColor1;//����ģʽ����ɫ1
	unsigned GridColor2;//����ģʽ����ɫ2
	CullMode CullMode;//�޳�ģʽ

	enum DrawLineMethodType { dlDDA, dlMidBresenham };

	///���߷���
	DrawLineMethodType DrawLineMethod;// 0 --DDA , 1 -- MidBresenham

	enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
	///������η���
	DrawPolygonMethodType DrawPolygonMethod; // 0 -- Xɨ���� �� 1 - ���Ա߱� �� 2--��Ե���

	//��������������ά�任����������
	int draw3D = 0;//0����ͨ�߿� 1��������ɫ
	struct MouseStruct3D
	{
		int oldX, oldY, oldZ;
		int newX, newY, newZ;
	}MouseSet3D;
	Sta();
	int Light = false;//�жϹ���
	double Ka;//������ϵ��
	double Kd;//������ϵ��
	double Ks;//����ϵ��
};

extern Sta g_Sta;

#endif
