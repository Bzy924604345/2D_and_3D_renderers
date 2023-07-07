#include <windows.h>
#include "MessageHandler.h"
#include "resource.h"
#include "Graphic.h"
#include "GeoDefine.h"
#include "GeoDefine3D.h"
#include "GeometryFactory.h"
#include "Renderer2D.h"
#include "Add2D.h"
#include "Editor.h"
#include "DialogHelper.h"
#include "LayerImporter.h"
#include "Sta.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Cut3D.h"
#include "Draw.h"
#include "Renderer3D.h"


enum OperationType {
	otNone, otSelect, otPan, otScaleOfCenter, otScaleOfFixedPoint,
	otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
	otDrawCircle, otDrawEllipse,
	otDrawPoint, otClear, otCut,
	ot_3DCUBE, ot3DRotate, ot_3DCUBE2,
	otFill
};

bool g_isEditMode = false;//�Ƿ�༭ģʽ
bool g_3dMode = false;//�ж�3Dģʽ
bool g_3dMode2 = false;//�ж�����ģʽ
bool g_3dbuild = false;//�жϳ���ͼ
OperationType g_OperationType = otNone;//��ǰ��������
Dataset g_Dataset;//���ݼ�
Layer* g_pLayer = NULL;
Add2D p_Renderer;
Editor g_Editor;//2D�༭��
Renderer3D g_Renderer3D;
GroupNode g_root;

vector<PixelPoint> Seed_Points;

void refreshMenuState();//���²˵�״̬
void buildScene(GroupNode* pRoot);//����ͼ
Layer* openLayer();

Mesh* g_pMesh = NULL;
Matrix4 g_modelMatrix;//3Dģ�ͱ任����
Matrix4 g_viewMatrix;//3D��ͼ�任����
Matrix4 g_projectMatrix;//3DͶӰ�任����
Matrix4 g_transformMatrix3D;//3D�ܱ任����

//Mesh g_tin;
//Vec3 g_eye(0, 0, 5), g_center(0, 0, 0), g_up(0, 1, 0);
//double D3SCALE = 1;
//Box3D g_box3D(-1, -1, -1, 1, 1, 1);

///����˵���Ϣ
void handleMenuMessage(int menuID)
{
	switch (menuID)
	{
	case ID_DRAW_1PIXEL:
		g_Sta.DrawMode = Sta::dmGrid;
		g_Sta.GridWidth = 1;
		g_Sta.GridHeight = 1;
		refreshWindow();
		break;
	case ID_DRAW_5PIXEL:
		g_Sta.DrawMode = Sta::dmGrid;
		g_Sta.GridWidth = 5;
		g_Sta.GridHeight = 5;
		refreshWindow();
		break;
	case ID_DRAW_10PIXEL:
		g_Sta.DrawMode = Sta::dmGrid;
		g_Sta.GridWidth = 10;
		g_Sta.GridHeight = 10;
		refreshWindow();
		break;
	case ID_DRAW_20PIXEL:
		g_Sta.DrawMode = Sta::dmGrid;
		g_Sta.GridWidth = 20;
		g_Sta.GridHeight = 20;
		refreshWindow();
		break;
	case ID_PIXEL_MODE:
		g_Sta.DrawMode = Sta::dmPixel;
		refreshWindow();

	case ID_ROAM_MODE:
		g_isEditMode = !g_isEditMode;
		g_OperationType = otSelect;
		setRubberMode(rmNone);
		setCursor(csArrow);
		break;
	case ID_PAN:
		g_OperationType = otPan;
		setRubberMode(rmNone);
		setCursor(csHand);
		break;
	case ID_2D_DRAW_POINT:
		g_OperationType = otDrawPoint;
		//g_isEditMode = true;
		setRubberMode(rmNone);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_RECT:
		g_OperationType = otDrawRectangle;
		//g_isEditMode = true;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_LINE:
		g_OperationType = otDrawLine;
		//g_isEditMode = true;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	case ID_2D_DRAW_POLYLINE:
		g_OperationType = otDrawPolyline;
		//g_isEditMode = true;
		setRubberMode(rmPolyline);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_POLYGONOUTLINE:
		g_OperationType = otDrawPolygonOutline;
		//g_isEditMode = true;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_POLYGON:
		g_OperationType = otDrawPolygon;
		//g_isEditMode = true;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_DARW_CIRCLE:
		g_OperationType = otDrawCircle;
		//g_isEditMode = true;
		setRubberMode(rmLine);
		setCursor(csCross);
		break;
	case ID_2D_DRAW_ELLIPSE:
		g_OperationType = otDrawEllipse;
		//g_isEditMode = true;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;

	case ID_RED:
		setPenColor(RED);
		refreshWindow();
		break;
	case ID_GREEN:
		setPenColor(GREEN);
		refreshWindow();
		break;
	case ID_BLUE:
		setPenColor(BLUE);
		refreshWindow();
		break;
	case ID_BLACK:
		setPenColor(BLACK);
		refreshWindow();
		break;
	case ID_ORANGE:
		setPenColor(ORANGE);
		refreshWindow();
		break;
	case ID_YELLOW:
		setPenColor(YELLOW);
		refreshWindow();
		break;
	case ID_CYAN:
		setPenColor(CYAN);
		refreshWindow();
		break;
	case ID_PURPLE:
		setPenColor(PURPLE);
		refreshWindow();
		break;

	case ID_2D_CLEAR:
		g_OperationType = otClear;
		g_Dataset.clear();
		Seed_Points.clear();
		//g_Sta.Cut = 0;
		refreshWindow();
		break;
	case ID_2D_CUT:
		g_OperationType = otCut;
		g_Sta.Cut = 1;
		setRubberMode(rmRectangle);
		setCursor(csCross);
		break;
	case ID_3D_WIREF:  //͸��ͶӰ
		buildScene(&g_root);
		g_OperationType = ot_3DCUBE;
		g_3dMode = true;
		g_3dMode2 = false;
		g_Sta.draw3D = 0;
		setCursor(csArrow);
		g_Sta.Light = false;
		refreshWindow();
		break;
	case ID_3D_WIREF2:  //����ͶӰ
		g_OperationType = ot_3DCUBE2;
		g_3dMode = true;
		g_3dMode2 = true;
		g_Sta.draw3D = 0;
		setCursor(csArrow);
		g_Sta.Light = false;
		refreshWindow();
		break;
	case ID_3D_CutWay:
		g_Sta.g_3d_CutWay = true;
	case ID_2D_FILL:
		g_OperationType = otFill;
		setRubberMode(rmNone);
		setCursor(csArrow);
		break;
	case ID_3D_BLANK:
		setRubberMode(rmNone);
		g_OperationType = ot_3DCUBE;
		g_3dMode = true;
		g_3dMode2 = false;
		g_Sta.draw3D = 1;
		g_Sta.Light = false;
		refreshWindow();
		break;
	case ID_3D_LIGHT_5:
		g_OperationType = ot_3DCUBE;
		g_3dMode = true;
		g_3dMode2 = false;
		g_Sta.draw3D = 1;
		g_Sta.Light = true;
		g_Sta.Ka = 0.5;
		g_Sta.Kd = 0.5;
		refreshWindow();
		break;
	case ID_3D_LIGHT_4:
		g_OperationType = ot_3DCUBE;
		g_3dMode = true;
		g_3dMode2 = false;
		g_Sta.draw3D = 1;
		g_Sta.Light = true;
		g_Sta.Ka = 0.5;
		g_Sta.Kd = 0.4;
		refreshWindow();
		break;
	case ID_3D_LIGHT_3:
		g_OperationType = ot_3DCUBE;
		g_3dMode = true;
		g_3dMode2 = false;
		g_Sta.draw3D = 1;
		g_Sta.Light = true;
		g_Sta.Ka = 0.4;
		g_Sta.Kd = 0.4;
		refreshWindow();
		break;
	case ID_CULL_NONE:
		g_Sta.CullMode = CULL_NONE;
		refreshWindow();
		break;
	case ID_CULL_CLOCKWISE:
		g_Sta.CullMode = CULL_CLOCKWISE;
		refreshWindow();
		break;
	case ID_CULL_COUNTERCLOCKWISE:
		g_Sta.CullMode = CULL_COUNTERCLOCKWISE;
		refreshWindow();
		break;

	case ID_ADD_LAYER:
	{
		Layer* pLayer = openLayer();
		if (pLayer)
		{
			if (g_Dataset.getLayerCount() == 1)//��ӵ�һ��ͼ��ʱ���ó�ʼ�任����ͱ�����
			{
				double x_center = /*-*/pLayer->envelop.centerX();
				double y_center = /*-*/pLayer->envelop.centerY();//��Χ�����ĵ�����

				p_Renderer.init(x_center, y_center, pLayer->envelop.width(), pLayer->envelop.height(),
					0,0,getWindowWidth(), getWindowHeight());
			}
			g_Dataset.addLayer(pLayer);
			refreshWindow();
		}
		break;
	}
	break;
	}
	refreshMenuState();//ˢ�²˵�״̬
}

///��������Ϣ
void handleKeyMessage(int key)
{
#pragma region 2D
	if (!g_3dMode)
	{
		double pan = 10.0;
		double angle = DEG_RAD(30);
		Matrix3 m;//ģ�;����۲�任���󣬸��ݱ༭ģʽ�����ģʽ����
		Color color = BLACK;
		switch (key)
		{
		case VK_UP: // ��һ�У��Ϲ��
		{
			m.makeTranslate(0, pan);
			refreshWindow();
			break;
		}
		case VK_DOWN:
		{
			m.makeTranslate(0, -pan);
			refreshWindow();
			break;
		}
		case VK_LEFT:
		{
			if (isCtrlKeyPressed()) {//������ת
				int x = getWindowWidth() / 2;
				int y = getWindowHeight() / 2;
				m.makeTranslate(-x, -y);
				m *= Matrix3::Rotate(angle);
				m *= Matrix3::translate(x, y);
			}
			else
			{
				m.makeTranslate(-pan, 0);
			}
			
			break;
		}
		case VK_RIGHT:
		{
			if (isCtrlKeyPressed())
			{
				int x = getWindowWidth() / 2;
				int y = getWindowHeight() / 2;
				m.makeTranslate(-x, -y);
				m *= Matrix3::Rotate(-angle);
				m *= Matrix3::translate(x, y);
			}
			else
			{
				m.makeTranslate(pan, 0);
			}
			refreshWindow();
			break;
		}
		default:
			return;
		}

		if (g_isEditMode) //�༭ģʽ��Ӧ��ģ�ͱ任�����޸�����
		{
			//Matrix3 m2 = p_Renderer.getPixelToWorldMatrix();
			//g_Editor.apply(&g_player, p_Renderer.mTransformMatrix * m * m2);
			g_Editor.apply(&g_Dataset,m);
		}
		else
		{
			//m = Matrix3::Inverse(m);
			p_Renderer.applyViewMatrix(m);
			//g_Renderer.applyViewMatrix(m);
		}

		refreshWindow();
		return;
	}
#pragma endregion

#pragma region 3D
	else
	{
		double angle = DEG_RAD(30);//PI / 16;
		double step = 10.0;
		Matrix4 m;
		switch (key)
		{
		case VK_UP:
		{
			if (isShiftKeyPressed())
				m.makeRotateX(-angle);
			else if (isCtrlKeyPressed())
				m.makeTranslate(0, 0, -step);
			else if (isCapsKeyPressed())
				m.makeShareY(0.2, 0);
			else
				m.makeTranslate(0, step, 0);
			refreshWindow();
		}
		break;
		case VK_DOWN:
		{
			if (isShiftKeyPressed())
				m.makeRotateX(angle);
			else if (isCtrlKeyPressed())
				m.makeTranslate(0, 0, step);
			else if (isCapsKeyPressed())
				m.makeShareY(-0.2, 0);
			else
				m.makeTranslate(0, -step, 0);
			refreshWindow();
		}
		break;
		case VK_LEFT:
		{
			if (isShiftKeyPressed())
				m.makeRotateZ(angle);
			else if (isCtrlKeyPressed())
				m.makeRotateY(angle);
			else if (isCapsKeyPressed())
				m.makeShareX(-0.2,0);
			else
				m.makeTranslate(-step, 0, 0);
			refreshWindow();
		}
		break;
		case VK_RIGHT:
		{
			if (isShiftKeyPressed())
				m.makeRotateZ(-angle);
			else if (isCtrlKeyPressed())
				m.makeRotateY(-angle);
			else if (isCapsKeyPressed())
				m.makeShareX(0.2, 0);
			else
				m.makeTranslate(step, 0, 0);
		}
		break;
		default:
			return;
		}
		g_Renderer3D.g_viewMatrix *= m;//�ֲ��任����m�˵���ͼ�任����

		refreshWindow();
	}
#pragma endregion
}

Geometry* createGeometry(OperationType operationType, vector<PixelPoint>& pts);

///���������Ϣ
void handleMouseMessage(UINT message, int x, int y, int det, WPARAM wParam, LPARAM lParam)
{
	int LOWORD(lParam);
	int HIWORD(lParam);
	DPtToLPt(x, y, x, y);//�����豸����תΪ�����߼�����

	static Point2D pt1;
	static bool dragging = false;
	static bool hasMoved = false;//����Ƿ��ƶ���
	Matrix3 m;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		if (g_OperationType == otPan)//��ǰ��������
		{
			pt1.x = x;
			pt1.y = y;
			dragging = true;
			hasMoved = false;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		hasMoved = true;
		if (dragging)
		{
			if (pt1.x != x || pt1.y != y)
			{
				m.makeTranslate(x - pt1.x, y - pt1.y);
				pt1.x = x;
				pt1.y = y;
				if (g_isEditMode)
				{
					g_Editor.apply(&g_Dataset, m);
				}
				else
				{
					//m = Matrix3::Inverse(m);
					p_Renderer.applyViewMatrix(m);
					//g_Renderer.applyViewMatrix(m);
				}
				refreshWindow();
			}
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		dragging = false;
		RubberMode rm = getRubberMode();
		if (rm == rmNone)
		{
			PixelPoint pt = { x,y };
			if (g_OperationType == otFill)
			{
				Seed_Points.push_back(pt);
			}
			else if (g_OperationType == otDrawPoint)
			{
				g_pLayer->addGeometry(GeometryFactory::createPointGeometry(x, y));
			}
			refreshWindow();
			break;
		}

		if (rm == rmNone || rm == rmPolygon) break;

		int c = getRubberPointCount();
		if (c == 2)
		{
			PixelPoint pt1, pt2;
			getRubberPoints(pt1, pt2);

			switch (g_OperationType)
			{
			case otCut:
				g_Sta.side.push_back(pt1);
				g_Sta.side.push_back(pt2);
				refreshWindow();
				break;
			case otDrawLine:
			case otDrawCircle:
			{
				if (pt1.x == pt2.x && pt1.y == pt2.y) return;

				if (g_OperationType == otDrawLine)
					g_pLayer->addGeometry(GeometryFactory::createPolylineGeometry(pt1, pt2));
				else
					g_pLayer->addGeometry(GeometryFactory::createCircleGeometry(pt1.x, pt1.y, pt2.x, pt2.y));
				refreshWindow();
			}
			break;
			case otDrawRectangle:
			case otDrawRectangleOutline:
			case otDrawEllipse:
			{
				double x1 = pt1.x;
				double y1 = pt1.y;
				double x2 = pt2.x;
				double y2 = pt2.y;
				if (x1 == x2 || y1 == y2) return;

				if (g_OperationType == otDrawRectangle)
					g_pLayer->addGeometry(GeometryFactory::creatRectangleGeometry(x1, y1, x2, y2));
				else if (g_OperationType == otDrawRectangleOutline)
					g_pLayer->addGeometry(GeometryFactory::creatRectangleOutlineGeometry(x1, y1, x2, y2));
				else
					g_pLayer->addGeometry(GeometryFactory::createEllipseGeometry(x1, y1, x2, y2));
				refreshWindow();
			}
			break;
			}
		}
	}
	break;
	case WM_RBUTTONUP:
	{
		dragging = false;

		RubberMode rm = getRubberMode();
		if (rm != rmPolyline && rm != rmPolygon) break;//����Ƥ��ģʽ���˳�

		int c = getRubberPointCount();

		//��Ƥ�߲�������ʱ�Żᴴ��ͼ�ζ��������ж���Ƥ�߲����Ƿ����
		switch (g_OperationType)
		{
		case otDrawEllipse:
			if (message == WM_RBUTTONUP)return;//����Ҽ�������Polygon��Polylineģʽ������־
			break;
		case otDrawPolyline:
		{
			if (c < 2) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			g_pLayer->addGeometry(GeometryFactory::createPolylineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}
		break;
		case otDrawPolygon:
		case otDrawPolygonOutline:
		{
			if (c < 3) return;

			vector<PixelPoint> pts(c);
			getRubberPoints(pts.data());

			//�����

			if (g_OperationType == otDrawPolygon)
				g_pLayer->addGeometry(GeometryFactory::createPolygonGeometry(pts.data(), pts.size()));
			else
				g_pLayer->addGeometry(GeometryFactory::createPolygonOutlineGeometry(pts.data(), pts.size()));

			refreshWindow();
		}



	//	if (message == WM_LBUTTONUP)
	//		return;//������������Line��Rectangeģʽ������־
	//	break;
	//	default:
	//		return;

	//	vector<PixelPoint> pts(c);
	//	getRubberPoints(pts.data());

	////	//�����

	//	//��Ƥ�߲���������������Ƥ�߶�������ͼ��
	//	Geometry* pGeometry = createGeometry(g_OperationType, pts);
	//	if (pGeometry)
	//	{
	//		g_pLayer->addGeometry(pGeometry);
	//		refreshWindow();		
	}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		short det = (short)HIWORD(wParam);
		if (!g_3dMode)//2D
		{
			if (isCtrlKeyPressed())
			{
				x = 0;
				y = 0;
			}
			double d = det > 0 ? 0.9 : 1.0 / 0.9;
			Matrix3 m;
			//int x0 = getWindowWidth() / 2;
			//int y0 = getWindowHeight() / 2;
			m.makeTranslate(-x, -y);
			m *= Matrix3::Scale(d, d);
			m *= Matrix3::translate(x, y);

			if (g_isEditMode)
			{
				g_Editor.apply(&g_Dataset, m);
			}
			else
			{
				//m = Matrix3::Inverse(m);
				p_Renderer.applyViewMatrix(m);
				//g_Renderer.applyViewMatrix(m);
			}
		}
		else//3D
		{
			double d = det > 0 ? -10.0 : 10.0;
			g_Renderer3D.g_viewMatrix *= Matrix4::translate(0, 0, d);//�ӵ�ǰ���ƶ�����zֵ
		}
		refreshWindow();
		break;
		//if (g_3dMode == true)
		//{
		//	//�����任
		//	short det = (short)HIWORD(wParam);
		//	Vec3 viewLine = g_eye - g_center;
		//	viewLine.normalize();
		//	Vec3 d = viewLine * 1.0;
		//	if (det > 0)
		//	{
		//		g_eye += d;
		//		g_center += d;
		//		D3SCALE *= 0.9;
		//	}
		//	else
		//	{
		//		g_eye -= d;
		//		g_center -= d;
		//		D3SCALE /= 0.9;
		//	}
		//}

	}
	break;
	}
}


//���������Ϣ
void display()
{
	setYUp(true);//y������
	setOrig(0, getWindowHeight()); //���½�
	//setOrig(getWindowWidth() / 2, getWindowHeight() / 2);// ԭ��Ϊ��������

	if (!g_3dMode)
	{
			p_Renderer.render(&g_Dataset); //����ģʽ
			
			for (int i = 0; i < Seed_Points.size(); ++i)  //���
			{
				PixelPoint seed = Seed_Points[i];
				Draw::Fill4(seed.x, seed.y, getPenColor(), RED);
			}
	}

	else if(g_3dMode)
	{
	    int width = getWindowWidth(), height = getWindowHeight();

	    if (width == 0 || height == 0)
			return;
		double g_fovy = 120, g_zNear = 10, g_zFar = 1600;

		if (g_3dMode2)//����ͶӰ
		{
			g_Renderer3D.g_projectMatrix.makeIdentity();
			g_Renderer3D.g_projectMatrix.makeOrtho(width, height, g_zNear, g_zFar);
			g_Renderer3D.renderSceneNode(&g_root, Matrix4());
		}
		else if (!g_3dMode2)//͸��ͶӰ
		{
			g_Renderer3D.renderSceneNode(&g_root, Matrix4());
		}
    }
}

///���ݲ�������operationType�Ͷ��㼯��pts�������ʵ�ͼ�ζ���
Geometry* createGeometry(OperationType operationType, vector<PixelPoint>& pts)
{
	int count = pts.size();
	if (count < 2) return NULL;

	PixelPoint* pData = pts.data();

	///��ͬ�������Ϳ�������ͬһ���͵�ͼ�Σ�����Rectangeģʽ���ɵ�ͼ��Ҳ�Ƕ����
	switch (operationType)
	{
	case otDrawLine:
		return GeometryFactory::createPolylineGeometry(pData, count);
	case otDrawRectangle:
		return GeometryFactory::creatRectangleGeometry(pData, count);
	case otDrawRectangleOutline:
		return GeometryFactory::creatRectangleOutlineGeometry(pData, count);
	case otDrawPolyline:
		return GeometryFactory::createPolylineGeometry(pData, count);
	case otDrawPolygon:
		return GeometryFactory::createPolygonGeometry(pData, count);
	case otDrawPolygonOutline:
		return GeometryFactory::createPolygonOutlineGeometry(pData, count);
	case otDrawCircle:
		return GeometryFactory::createCircleGeometry(pData, count);
	case otDrawEllipse:
		return GeometryFactory::createEllipseGeometry(pData, count);
	}
	return NULL;
}


///ѡ�񲢴�ͼ��
Layer* openLayer()
{
	char path[MAX_PATH] = { 0 };
	if (DialogHelper::selectSingleFile("", "shp", path, MAX_PATH))
	{
		return LayerImporter::importShpLayer(path);
	}
	return NULL;
}


#pragma region less-used 

double g_fovy = 60, g_zNear = 10, g_zFar = 1600;

///�����ڴ�С�仯��Ϣ
void sized(int cx, int cy)
{
	if (cx == 0 || cy == 0)return;
	double aspectRatio = (double)cx / cy;

	//��˼������θ���ͶӰ����ֱ�ӵõ�Զ��ƽ�����
	//g_projectMatrix.makePerspective(g_fovy, aspectRatio, g_zNear, g_zFar);
}


///��ʼ��
void initialize()
{
	g_pLayer = new Layer();
	g_Dataset.addLayer(g_pLayer);

	int width = getWindowWidth(), height = getWindowHeight();
	if (width == 0 || height == 0) return;

	double aspectRatio = (double)width / height;

	g_Renderer3D.g_projectMatrix.makePerspective(g_fovy, aspectRatio, g_zNear, g_zFar);//y�����Žǣ���߱ȣ���ƽ�棬Զƽ��
	//�۲�����ϵ����������ϵ�ж���
	Vec3 eye(600, 600, 300), center(0, 0, 0), up(0, 1, 0);//��������������
	g_Renderer3D.LookAt(eye, center, up);
	//����ʹ�õ���������ϵ��λ�����أ��任�����ʱ��Ҫת��Ϊ�ӿ���������
	g_pMesh = Box3D().toMesh();
	g_Renderer3D.g_modelMatrix.makeScale(100, 100, 100);
	g_Renderer3D.g_modelMatrix *= Matrix4::translate(300, 300, -100);
	//��������ͼ
	buildScene(&g_root);
	
	refreshMenuState();
}

///���ٴ���ʱ��������
void destroy()
{
	delete g_pMesh;
}

#pragma endregion

/// <summary>
/// ���²˵�״̬
/// </summary>
void refreshMenuState()
{
	void checkMenuItem(int menuID, bool checked);
	void setMenuItemText(int menuID, const char* text);

	//�޸Ĳ˵�������
	setMenuItemText(ID_ROAM_MODE, g_isEditMode ? "�༭ģʽ������л������ģʽ��" : "���ģʽ������л����༭ģʽ��");

}
void setMenuItemText(int menuID, const char* text)
{
	extern HWND g_hWnd;
	HMENU hMenu = GetMenu(g_hWnd);
	ModifyMenuA(hMenu, menuID, MF_BYCOMMAND | MF_STRING, menuID, text);
}

//����ͼ
void buildScene(GroupNode* pRoot)
{
	{
		TransformNode* pTransformNode = new TransformNode(pRoot);
		pTransformNode->transformMatrix.makeScale(100, 100, 100);
		pTransformNode->transformMatrix *= Matrix4::translate(100, 0, 0);

		ModelNode* pModelNode = new ModelNode(pTransformNode);
		pModelNode->addRenderable(new Material, Box3D().toMesh());
	}
	{
		TransformNode* pTransformNode = new TransformNode(pRoot);
		pTransformNode->transformMatrix.makeScale(50,50,50);
		pTransformNode->transformMatrix *= Matrix4::translate(150, 200, 30);

		ModelNode* pModelNode = new ModelNode(pTransformNode);
		pModelNode->addRenderable(new Material, Box3D().toMesh());
	}
	{
		TransformNode* pTransformNode = new TransformNode(pRoot);
		pTransformNode->transformMatrix.makeScale(50, 50, 50);
		pTransformNode->transformMatrix *= Matrix4::translate(150, 0, 250);

		ModelNode* pModelNode = new ModelNode(pTransformNode);
		pModelNode->addRenderable(new Material, Box3D().toMesh());
	}
}

#pragma region handleMessage

///������Ϣ����ڵ�
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://�˵���Ϣ
	{
		int menuID = LOWORD(wParam);
		handleMenuMessage(menuID);
		return FALSE;
	}
	case WM_PAINT://������Ϣ
		display();
		return FALSE;
	case WM_CREATE:
		init((unsigned)hWnd);
		initialize();
		return TRUE;
	case WM_DESTROY:
		destroy();
		PostQuitMessage(0);
		return TRUE;
	case WM_KEYDOWN://������Ϣ
	{
		int key = wParam;//��ð�������
		handleKeyMessage(key);
		return FALSE;
	}
	case WM_MOUSEMOVE://����ƶ���Ϣ
	case WM_LBUTTONDOWN://������������Ϣ	
	case WM_LBUTTONUP://������������Ϣ
	case WM_RBUTTONUP://����Ҽ�������Ϣ
	case WM_RBUTTONDOWN://����Ҽ�������Ϣ
	case WM_LBUTTONDBLCLK://���˫����Ϣ
	case WM_MOUSEWHEEL://��������Ϣ
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		int det = message == WM_MOUSEWHEEL ? (short)HIWORD(wParam) : 0;
		handleMouseMessage(message, x, y, det, wParam, lParam);
		return FALSE;
	}
	case WM_SIZE://���ڴ�С�仯��Ϣ
	{
		UINT cx = LOWORD(lParam);
		UINT cy = HIWORD(lParam);
		sized(cx, cy);
		return TRUE;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
}

#pragma endregion