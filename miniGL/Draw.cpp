#include <iostream>
#include <windows.h>
#include "Graphic.h"
#include "Draw.h"
#include "Sta.h"
#include <vector>
#include "Vec4.h"


void Draw::PointCut(Vec3 v, int x1, int y1, int x2, int y2)
{
	// 确定裁剪范围
	PixelPoint LeftUp, RightDown;
	PixelPoint LeftDown, RightUp;
	if (x1 < x2)
	{
		if (y1 > y2)
		{
			LeftUp.x = x1;
			LeftUp.y = y1;
			LeftDown.x = x1;
			LeftDown.y = y2;
			RightUp.x = x2;
			RightUp.y = y1;
			RightDown.x = x2;
			RightDown.y = y2;
		}
		else
		{
			LeftUp.x = x1;
			LeftUp.y = y2;
			LeftDown.x = x1;
			LeftDown.y = y1;
			RightUp.x = x2;
			RightUp.y = y2;
			RightDown.x = x2;
			RightDown.y = y1;
		}
	}
	else
	{
		if (y1 < y2)
		{
			LeftUp.x = x2;
			LeftUp.y = y2;
			LeftDown.x = x2;
			LeftDown.y = y1;
			RightUp.x = x1;
			RightUp.y = y2;
			RightDown.x = x1;
			RightDown.y = y1;
		}
		else
		{
			LeftUp.x = x2;
			LeftUp.y = y1;
			LeftDown.x = x2;
			LeftDown.y = y2;
			RightUp.x = x1;
			RightUp.y = y1;
			RightDown.x = x1;
			RightDown.y = y2;
		}
	}
	drawrect(LeftUp.x, LeftUp.y, RightDown.x, RightDown.y);
	if (v.x() > LeftUp.x && v.x() < RightUp.x && v.y() < LeftUp.y && v.y() > LeftDown.y)
	{
		setPixel(v.x(), v.y(), getPenColor());//绘制
	}
}

void Draw::drawLine(int x0, int y0, int x1, int y1, unsigned color)
{
	int delta_x = abs(x1 - x0), delta_y = abs(y1 - y0), flag = 0, temp;
	if (delta_x < delta_y)
	{
		temp = delta_x;
		delta_x = delta_y;
		delta_y = temp;
		flag = 1;
	}

	int dx, dy;
	if (abs(x1 - x0) != 0) 
		dx = abs(x1 - x0) / (x1 - x0);
	else
		dx = 0;
	if (abs(y1 - y0) != 0) 
		dy = abs(y1 - y0) / (y1 - y0);
	else
		dy = 0;

	int d = delta_x - 2 * delta_y, x = x0, y = y0;

	for (int i = 0; i <= delta_x; ++i)
	{
		if (g_Sta.DrawMode == Sta::dmGrid)
		{
			Draw::drawCell(y / g_Sta.GridHeight, x / g_Sta.GridHeight, color);
		}
		else
		{

			setPixel(x, y, color);

		}
		if (d < 0)
		{
			x += dx, y += dy, d += 2 * delta_x - 2 * delta_y;
		}
		else
		{
			if (flag == 1) y += dy;
			else x += dx;
			d -= 2 * delta_y;
		}
	}
}

void Draw::drawPolyline(PixelPoint polypoint[], int size, unsigned color)
{
	for (int i = 0; i < size - 1; i++)
	{
		Draw::drawLine(polypoint[i].x, polypoint[i].y, polypoint[i + 1].x, polypoint[i + 1].y,getPenColor());
	}
}


#pragma region Draw
//定义活性边表AET和新边表NET
typedef struct ET
{
	float x;
	float dx, ymax;
	ET* next;
} AET, NET;


void Draw::PolyScan(PixelPoint polypoint[], int size, unsigned color)
{
	//计算边界
	int MaxY = polypoint[0].y, MinY = polypoint[0].y;
	int i;
	for (i = 0; i < size; i++)
	{
		if (polypoint[i].y > MaxY) MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY) MinY = polypoint[i].y;
	}

	//初始化AET表
	AET* pAET = new AET;
	pAET->next = NULL;

	//初始化NET表
	NET* pNET[4000];
	for (i = 0; i <= MaxY - MinY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}

	//建立NET表
	for (i = 0; i <= MaxY - MinY; i++)
	{
		for (int j = 0; j < size; j++)
			if (polypoint[j].y == i + MinY)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
				if (polypoint[(j - 1 + size) % size].y > polypoint[j].y)
				{
					NET* p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + size) % size].y;
					p->dx = float((polypoint[(j - 1 + size) % size].x - polypoint[j].x)) /
						(polypoint[(j - 1 + size) % size].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}

				if (polypoint[(j + 1 + size) % size].y > polypoint[j].y)
				{
					NET* p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + size) % size].y;
					p->dx = float((polypoint[(j + 1 + size) % size].x - polypoint[j].x)) /
						(polypoint[(j + 1 + size) % size].y - polypoint[j].y);
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}

	//更新活性边表AET
	for (i = 0; i <= MaxY - MinY; i++)
	{
		//计算新的交点x,更新AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}

		//更新后,新AET先排序
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET* s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}

		//(改进算法)先从AET表中删除ymax==i的结点
		AET* q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i + MinY)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}

		//将NET中的新点加入AET,并按X值递增排序
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET* s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}

		p = pAET->next;
		while (p && p->next)
		{
			for (float j = p->x; j < p->next->x; j++)
			{
				if (g_Sta.DrawMode == Sta::dmPixel) setPixel(j, i + MinY, color);
				else Draw::drawCell((i + MinY) / g_Sta.GridWidth, j / g_Sta.GridHeight, color);
			}
			p = p->next->next;//考虑端点情况
		}
	}
}
#pragma endregion

#pragma region Draw
void Draw::drawPoint(int x, int y, unsigned color)
{
	if (g_Sta.DrawMode == Sta::dmGrid)
	{
		x = x / g_Sta.GridWidth;
		y = y / g_Sta.GridHeight;
		drawCell(y, x, color);
	}
	else setPixel(x, y, RED);

}

void Draw::drawGrid()
{
	int orig_x, orig_y;
	getOrig(orig_x, orig_y);

	int left = (int)((float)orig_x / g_Sta.GridWidth + 0.5);
	int right = (int)((float)(getWindowWidth() - orig_x) / g_Sta.GridWidth + 0.5);
	int top = (int)((float)orig_y / g_Sta.GridHeight + 0.5);
	int bottom = (int)((float)(getWindowHeight() - orig_y) / g_Sta.GridHeight + 0.5);

	int ymin, ymax;
	if (isYUp())
	{
		ymin = -bottom;
		ymax = top;
	}
	else
	{
		ymin = -top;
		ymax = bottom;
	}
	for (int col = -left; col <= right; ++col)
		for (int row = ymin; row <= ymax; ++row)
		{
			drawCell(row, col, (row + col) % 2 ? g_Sta.GridColor1 : g_Sta.GridColor2);
			//drawCell(row, col, PURPLE);
		}
}

void Draw::drawCell(int row, int col, unsigned color)
{
	for (int i = row * g_Sta.GridHeight; i < (row + 1) * g_Sta.GridHeight; ++i)
		for (int j = col * g_Sta.GridWidth; j < (col + 1) * g_Sta.GridWidth; ++j)
		{
			setPixel(j, i, color);
		}
}

//
//void Draw::drawPolygonOutline(PixelPoint pts[], int size, unsigned color)
//{
//	for (int i = 0; i < size - 1; ++i)
//	{
//		Draw::drawLine(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, color);
//	}
//	Draw::drawLine(pts[size - 1].x, pts[size - 1].y, pts[0].x, pts[0].y, color);
//}

void Draw::drawCircle(double x0, double y0, double r, unsigned color)
{

	double x = x0;
	double y = y0 + r;
	while (x - x0 < (r / sqrt(2) + 1))
	{
		double dx = x - x0, dy = y - y0;
		double d = pow((dx + 1), 2) + pow((dy - 0.5), 2) - r * r;
		if (g_Sta.DrawMode == Sta::dmGrid)
		{
			int a = x0 / g_Sta.GridWidth;
			int b = y0 / g_Sta.GridHeight;
			int c = dx / g_Sta.GridWidth;
			int d = dy / g_Sta.GridHeight;
			Draw::drawCell(b + d, a + c, color);
			Draw::drawCell(b + d, a - c, color);
			Draw::drawCell(b - d, a + c, color);
			Draw::drawCell(b - d, a - c, color);
			Draw::drawCell(b + c, a + d, color);
			Draw::drawCell(b + c, a - d, color);
			Draw::drawCell(b - c, a + d, color);
			Draw::drawCell(b - c, a - d, color);
		}
		else
		{
			setPixel(x0 + dx, y0 + dy, color);
			setPixel(x0 + dx, y0 + -dy, color);
			setPixel(x0 + -dx, y0 + dy, color);
			setPixel(x0 + -dx, y0 + -dy, color);
			setPixel(x0 + dy, y0 + dx, color);
			setPixel(x0 + dy, y0 + -dx, color);
			setPixel(x0 + -dy, y0 + dx, color);
			setPixel(x0 + -dy, y0 + -dx, color);
		}

		if (d < 0)
		{
			d += 2 * dx + 3;
			x += 1;
		}
		else
		{
			d += 2 * (dx - dy) + 5;
			x += 1;
			y -= 1;
		}
	}
}

void Draw::drawEllipse(double x0, double y0, double width, double height, unsigned color)
{
	double x = x0, a = width / 2, b = height / 2;
	double y = y0 + b;
	double d = b * b + a * a * (0.25 - b);

	while (b * b * (x - x0) < a * a * (y - y0))
	{
		double dx = x - x0, dy = y - y0;
		double d = b * b * pow((dx + 1), 2) + a * a * pow((dy - 0.5), 2) - a * a * b * b;
		if (g_Sta.DrawMode == Sta::dmGrid)
		{
			int a = x0 / g_Sta.GridWidth;
			int b = y0 / g_Sta.GridHeight;
			int c = dx / g_Sta.GridWidth;
			int d = dy / g_Sta.GridHeight;
			Draw::drawCell(b + d, a + c, color);
			Draw::drawCell(b + d, a - c, color);
			Draw::drawCell(b - d, a + c, color);
			Draw::drawCell(b - d, a - c, color);
		}
		else
		{
			setPixel(x0 + dx, y0 + dy, color);
			setPixel(x0 + dx, y0 - dy, color);
			setPixel(x0 - dx, y0 + dy, color);
			setPixel(x0 - dx, y0 - dy, color);
		}
		if (d < 0)
		{
			x += 1;
			d += b * b * (2 * dx + 3);
		}
		else
		{
			x += 1;
			y -= 1;
			d += b * b * (2 * dx + 3) + a * a * (2 - 2 * dy);
		}
	}
	while ((x - x0) >= 0 && (y - y0) >= 0)
	{
		double dx = x - x0, dy = y - y0;
		double d = b * b * pow((dx + 0.5), 2) + a * a * pow((dy - 1), 2) - a * a * b * b;
		if (g_Sta.DrawMode == Sta::dmPixel)
		{
			setPixel(x0 + dx, y0 + dy, color);
			setPixel(x0 + dx, y0 - dy, color);
			setPixel(x0 - dx, y0 + dy, color);
			setPixel(x0 - dx, y0 - dy, color);
		}
		else
		{
			int a = x0 / g_Sta.GridWidth;
			int b = y0 / g_Sta.GridHeight;
			int c = dx / g_Sta.GridWidth;
			int d = dy / g_Sta.GridHeight;
			Draw::drawCell(b + d, a + c, color);
			Draw::drawCell(b + d, a - c, color);
			Draw::drawCell(b - d, a + c, color);
			Draw::drawCell(b - d, a - c, color);
		}
		if (d < 0)
		{
			x += 1;
			y -= 1;
			d += 2 * b * b * (dx + 1) + a * a * (3 - dy);
		}
		else
		{
			y -= 1;
			d += a * a * (3 - 2 * dy);
		}
	}

}
#pragma endregion

//边界填充
void Draw::Fill4(int x, int y, unsigned bcolor, unsigned fcolor)
{
	if (getPixel(x, y) != bcolor && getPixel(x, y) != fcolor)
	{
		setPixel(x, y, fcolor);
		Draw::Fill4(x, y + 1, bcolor, fcolor);
		Draw::Fill4(x, y - 1, bcolor, fcolor);
		Draw::Fill4(x - 1, y, bcolor, fcolor);
		Draw::Fill4(x + 1, y, bcolor, fcolor);
	}
}

//剔除
bool Draw::cull(Vec4 p1, Vec4 p2, Vec4 p3)
{
	if (g_Sta.CullMode == CULL_CLOCKWISE)
	{
		if ((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p3.x() - p1.x()) * (p2.y() - p1.y()) < 0)
			return 1;
		else
			return 0;
	}
	else if (g_Sta.CullMode == CULL_COUNTERCLOCKWISE)
	{
		if ((p2.x() - p1.x()) * (p3.y() - p1.y()) - (p3.x() - p1.x()) * (p2.y() - p1.y()) > 0)
			return 1;
		else
			return 0;
	}
}

//消隐光照模型
void Draw::Setbottriangle3d(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int r, int g, int b, Vec3 a)
{
	double z_stepy = (double)(abs((z2 - z1) / (y2 - y1)));
	double z_stepy1 = (double)(abs((z3 - z1) / (y3 - y1)));

	double zy_xleft = z2;
	double zy_xright = z3;
	double z = z2;
	double xMax = 0;
	double xMin = 0;
	int t = 0;

	for (int now_y = y2; now_y < (y1 - 0.5); now_y++)
	{
		double xleft, xringht;
		xleft = (now_y - y1) * (x2 - x1) / (y2 - y1) + x1;
		xringht = (now_y - y1) * (x3 - x1) / (y3 - y1) + x1;
		double z_stepx = (double)abs((zy_xright - zy_xleft) / (xringht - xleft));
		z = zy_xleft;
		if (xleft > xringht)
		{
			xMax = xleft;
			xMin = xringht;
			t = 1;
		}
		if (xleft < xringht)
		{
			xMax = xringht;
			xMin = xleft;
			t = 0;
		}
		for (int now_x = xMin; now_x < (xMax); now_x++)
		{
			double I = 1;
			double n = 50;
			if (g_Sta.Light == true)
			{
				//漫反射计算
				Vec3 sd; //点光源位置
				sd.v[0] = 500;
				sd.v[1] = 500;
				sd.v[2] = -500;
				Vec3 yd; //像素点
				yd.v[0] = now_x;
				yd.v[1] = now_y;
				yd.v[2] = z;
				Vec3 c;//视线向量
				c.v[0] = now_x - sd.v[0];
				c.v[1] = now_y - sd.v[1];
				c.v[2] = z - sd.v[2];
				//double L1 = Vec3::Distance(yd, sd);
				//double L2 = Vec3::Distance(a, yd);
				//double cheng = c.v[0] * a.v[0] + c.v[1] * a.v[1] + c.v[2] * a.v[2];
				//double cos = cheng / (L1 * L2);
				Vec3 c1;
				c1 = c.normalize();
				a = a.normalize();
				double cheng = c1.v[0] * a.v[0] + c1.v[1] * a.v[1] + c1.v[2] * a.v[2];

				//镜面反射计算
				Vec3 V;  //视线方向
				V.v[0] = 600 - now_x;
				V.v[1] = 600 - now_y;
				V.v[2] = 300 - z;
				Vec3 H;//视线方向和入射方向中值
				H = c + V;
				H = H.normalize();
				//每顶点的视线方向和入射方向中值与法向量点积得cos2
				double cheng2 = H.v[0] * a.v[0] + H.v[1] * a.v[1] + H.v[2] * a.v[2];
				//总光强
				I = I * g_Sta.Ka + I * g_Sta.Kd * cheng + 1 * g_Sta.Ks * pow(cheng2, n);
			}
			setPixel(now_x, now_y, z, _RGB(r * I, g * I, b * I));

			if (t)
			{
				if (zy_xleft < zy_xright)
				{
					z -= z_stepx;
				}
				else {
					z += z_stepx;
				}
			}
			else
			{
				if (zy_xleft < zy_xright)
				{
					z += z_stepx;
				}
				else {
					z -= z_stepx;
				}
			}
		}
		if (z1 < z2)zy_xleft -= z_stepy;
		if (z1 > z2)zy_xleft += z_stepy;
		if (z1 < z3)zy_xright -= z_stepy1;
		if (z1 > z3)zy_xright += z_stepy1;
	}
}

void Draw::Settoptriangle3d(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int r, int g, int b, Vec3 a)
{
	double z_stepy = (double)abs((z2 - z1) / (y2 - y1));
	double z_stepy1 = (double)abs((z3 - z1) / (y3 - y1));
	double zy_xleft = z2;
	double zy_xright = z3;
	double z = z2;
	double xMax = 0;
	double xMin = 0;
	int t = 0;
	for (int now_y = y2; now_y > (y1 + 0.5); now_y--)
	{
		double xleft, xringht;
		xleft = (now_y - y1) * (x2 - x1) / (y2 - y1) + x1;
		xringht = (now_y - y1) * (x3 - x1) / (y3 - y1) + x1;
		double z_stepx = (double)abs((zy_xright - zy_xleft) / (xringht - xleft));
		z = zy_xright;
		if (xleft > xringht)
		{
			xMax = xleft;
			xMin = xringht;
			t = 1;
		}

		if (xleft < xringht)
		{
			xMax = xringht;
			xMin = xleft;
			t = 0;
		}
		for (int now_x = xMin; now_x < (xMax); now_x++)
		{
			double I = 1;
			double n = 50;
			if (g_Sta.Light == true)
			{
				//漫反射计算
				Vec3 sd; //点光源位置
				sd.v[0] = 500;
				sd.v[1] = 500;
				sd.v[2] = -500;
				Vec3 yd; //像素点
				yd.v[0] = now_x;
				yd.v[1] = now_y;
				yd.v[2] = z;
				Vec3 c;//视线向量
				c.v[0] = now_x - sd.v[0];
				c.v[1] = now_y - sd.v[1];
				c.v[2] = z - sd.v[2];
				//double L1 = Vec3::Distance(yd, sd);
				//double L2 = Vec3::Distance(a, yd);
				//double cheng = c.v[0] * a.v[0] + c.v[1] * a.v[1] + c.v[2] * a.v[2];
				//double cos = cheng / (L1 * L2);
				Vec3 c1;
				c1 = c.normalize();
				a = a.normalize();
				double cheng = c1.v[0] * a.v[0] + c1.v[1] * a.v[1] + c1.v[2] * a.v[2];

				//镜面反射计算
				Vec3 V;  //视线方向
				V.v[0] = 600 - now_x;
				V.v[1] = 600 - now_y;
				V.v[2] = 300 - z;
				Vec3 H;//视线方向和入射方向中值
				H = c + V;
				H = H.normalize();
				//每顶点的视线方向和入射方向中值与法向量点积得cos2
				double cheng2 = H.v[0] * a.v[0] + H.v[1] * a.v[1] + H.v[2] * a.v[2];
				//总光强
				I = I * g_Sta.Ka + I * g_Sta.Kd * cheng + 1 * g_Sta.Ks * pow(cheng2, n);
			}

			setPixel(now_x, now_y, z, _RGB(r * I, g * I, b * I));
			if (t)
			{
				if (zy_xleft < zy_xright)
				{
					z -= z_stepx;
				}
				else {
					z += z_stepx;
				}
			}
			else
			{
				if (zy_xleft < zy_xright)
				{
					z += z_stepx;
				}
				else {
					z -= z_stepx;
				}
			}
		}
		if (z1 < z2)zy_xleft -= z_stepy;
		if (z1 > z2)zy_xleft += z_stepy;
		if (z1 < z3)zy_xright -= z_stepy1;
		if (z1 > z3)zy_xright += z_stepy1;
	}
}

void Draw::Setgeneraltriangle3d(point3D A, point3D B, point3D C, int r, int g, int b, Vec3 a)
{
	double xlongside;//用来记录中间点对应的y值与长边的交点
	double x1 = A.x, y1 = A.y, z1 = A.z;
	double x2 = B.x, y2 = B.y, z2 = B.z;
	double x3 = C.x, y3 = C.y, z3 = C.z;

	if (y2 == y3)
	{
		if (y1 > y2)
		{
			Draw::Setbottriangle3d(x1, y1, z1, x2, y2, z2, x3, y3, z3, r, g, b, a);//平底三角形
		}
		if (y1 < y2)
		{
			Draw::Settoptriangle3d(x1, y1, z1, x2, y2, z2, x3, y3, z3, r, g, b, a);//平顶三角形
		}
	}
	else if (y1 == y3)
	{
		if (y2 > y1)
		{
			Draw::Setbottriangle3d(x2, y2, z2, x1, y1, z1, x3, y3, z3, r, g, b, a);
		}
		if (y2 < y1)
		{
			Draw::Settoptriangle3d(x2, y2, z2, x1, y1, z1, x3, y3, z3, r, g, b, a);
		}
	}
	else if (y1 == y2)
	{
		if (y3 > y1)
		{
			Draw::Setbottriangle3d(x3, y3, z3, x2, y2, z2, x1, y1, z1, r, g, b, a);
		}
		if (y3 < y1)
		{
			Draw::Settoptriangle3d(x3, y3, z3, x2, y2, z2, x1, y1, z1, r, g, b, a);
		}
	}
	else if (y1 == y2 && y2 == y3)
		int abbbb = 0;
	else
	{
		double xtop, ytop, xmiddle, ymiddle, xbottom, ybottom, ztop, zmiddle, zbottom;
		if (y1 < y2 && y2 < y3)
		{
			xtop = x1; xmiddle = x2; xbottom = x3;
			ytop = y1, ymiddle = y2; ybottom = y3;
			ztop = z1; zmiddle = z2; zbottom = z3;
		}
		else if (y1 < y3 && y3 < y2)
		{
			xtop = x1; xmiddle = x3; xbottom = x2;
			ytop = y1; ymiddle = y3; ybottom = y2;
			ztop = z1; zmiddle = z3; zbottom = z2;
		}
		else if (y2 < y1 && y1 < y3)
		{
			xtop = x2; xmiddle = x1; xbottom = x3;
			ytop = y2; ymiddle = y1; ybottom = y3;
			ztop = z2; zmiddle = z1; zbottom = z3;
		}
		else if (y2 < y3 && y3 < y1)
		{
			xtop = x2; xmiddle = x3; xbottom = x1;
			ytop = y2; ymiddle = y3; ybottom = y1;
			ztop = z2; zmiddle = z3; zbottom = z1;
		}
		else if (y3 < y1 && y1 < y2)
		{
			xtop = x3; xmiddle = x1; xbottom = x2;
			ytop = y3; ymiddle = y1; ybottom = y2;
			ztop = z3; zmiddle = z1; zbottom = z2;
		}
		else if (y3 < y2 && y2 < y1)
		{
			xtop = x3; xmiddle = x2; xbottom = x1;
			ytop = y3; ymiddle = y2; ybottom = y1;
			ztop = z3; zmiddle = z2; zbottom = z1;
		}
		xlongside = (ymiddle - ytop) * (xbottom - xtop) / (ybottom - ytop) + xtop;
		double z_longside_stepy = (double)abs(ztop - zbottom) / (ytop - ybottom);
		double z_longside_intersecty;
		if (ztop <= zbottom) {
			z_longside_intersecty = zbottom - (ymiddle - ybottom) * z_longside_stepy;
		}
		else {
			z_longside_intersecty = zbottom + (ymiddle - ybottom) * z_longside_stepy;
		}
		if (xlongside >= xmiddle)//左三角形
		{
			Draw::Setbottriangle3d(xbottom, ybottom, zbottom, xmiddle, ymiddle, zmiddle, xlongside, ymiddle, z_longside_intersecty, r, g, b, a);
			Draw::Settoptriangle3d(xtop, ytop, ztop, xlongside, ymiddle, z_longside_intersecty, xmiddle, ymiddle, zmiddle, r, g, b, a);
		}
		if (xlongside < xmiddle)//右三角形
		{
			Draw::Setbottriangle3d(xbottom, ybottom, zbottom, xlongside, ymiddle, z_longside_intersecty, xmiddle, ymiddle, zmiddle, r, g, b, a);
			Draw::Settoptriangle3d(xtop, ytop, ztop, xmiddle, ymiddle, zmiddle, xlongside, ymiddle, z_longside_intersecty, r, g, b, a);
		}
	}

}