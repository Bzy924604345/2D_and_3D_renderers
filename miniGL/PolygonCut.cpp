#include <iostream>
#include <windows.h>
#include "Graphic.h"
#include "Draw.h"
#include "Sta.h"
#include <vector>

using namespace std;

// 裁剪点链表结构体
typedef struct CET
{
	float x;
	float y;
	CET* next;
};

// 编码组结构体，记录点坐标以及水平竖直编码
typedef struct codePixelPoint
{
	PixelPoint p;
	int sp;
	int sz;
};

//水平编码函数,左-1，中0，右1
int spcode(PixelPoint p, PixelPoint LeftUp, PixelPoint RightDown)
{
	int c;
	if (p.x < LeftUp.x) 
		c = -1;
	else if (p.x > RightDown.x) 
		c = 1;
	else 
		c = 0;
	return c;
}

//竖直编码函数，下-1，中0，右1
int szcode(PixelPoint p, PixelPoint LeftUp, PixelPoint RightDown)
{
	int c;
	if (p.y < RightDown.y) 
		c = -1;
	else if (p.y > LeftUp.y) 
		c = 1;
	else 
		c = 0;

	return c;
}

// 求交点
PixelPoint Draw::jiaodian(PixelPoint firstpoint, PixelPoint secondpoint, PixelPoint s1, PixelPoint s2)
{
	PixelPoint jiaodian;
	double k;
	if (s1.x == s2.x) //竖直线
	{
		if (firstpoint.x == secondpoint.x)  // 同为竖直线
		{
		}
		else
		{
			k = ((double)(firstpoint.y) - secondpoint.y) / (firstpoint.x - secondpoint.x);
			double y = k * (s1.x - firstpoint.x) + firstpoint.y;
			jiaodian.x = s1.x;
			jiaodian.y = (int)(y);
		}
	}


	if (s1.y == s2.y) // 水平线
	{
		if (firstpoint.y == secondpoint.y) // 同为水平线
		{
		}
		else
		{
			k = ((double)(firstpoint.x) - secondpoint.x) / (firstpoint.y - secondpoint.y);
			double x = k * (s1.y - firstpoint.y) + firstpoint.x;
			jiaodian.x = (int)(x);
			jiaodian.y = s1.y;
		}
	}

	return jiaodian;
}

void Draw::PolygonCutSH(PixelPoint polypoint[], int size, int x1, int y1, int x2, int y2)
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

	//编码
	vector<codePixelPoint> codepolypoint(size);
	for (int i = 0; i < size; i++)
	{
		codepolypoint[i].p = polypoint[i];
		codepolypoint[i].sp = spcode(polypoint[i], LeftUp, RightDown);
		codepolypoint[i].sz = szcode(polypoint[i], LeftUp, RightDown);
	}

	size++;
	codepolypoint.resize(size);
	codepolypoint[size - 1].p = polypoint[0];
	codepolypoint[size - 1].sp = spcode(polypoint[0], LeftUp, RightDown);
	codepolypoint[size - 1].sz = szcode(polypoint[0], LeftUp, RightDown);

	int count = 0;//裁剪后边数
	CET* pET = new CET;	//初始化裁剪边表
	pET->next = NULL;

	//左边界裁剪
	for (int i = 0, j = 1; i < size - 1; i++, j++)
	{
		if (codepolypoint[i].sp == -1) //起点在外侧
		{
			if (codepolypoint[j].sp >= 0)	//外内，输出交点与终点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftUp, LeftDown);

				CET* p = new CET;
				CET* q = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
				count++;
			}
			// 外外不输出
		}
		else      //起点在内侧
		{
			if (codepolypoint[j].sp == -1)	//内外，输出交点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftUp, LeftDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //内内，输出终点
			{
				CET* q = new CET;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
			}
		}
	}

	count++;
	// 重置编码矩阵
	codepolypoint.clear();
	codepolypoint.resize(count);
	pET = pET->next; // 指向最后一个点


	for (int i = 0; i < count; i++)
	{
		if (pET)
		{
			codepolypoint[i].p.x = pET->x;
			codepolypoint[i].p.y = pET->y;
			codepolypoint[i].sp = spcode(codepolypoint[i].p, LeftUp, RightDown);
			codepolypoint[i].sz = szcode(codepolypoint[i].p, LeftUp, RightDown);
			pET = pET->next;
		}
		else codepolypoint[i] = codepolypoint[0];
	}



	CET* lET = new CET;		//初始化更新边表
	lET->next = NULL;
	pET = lET;
	count = 0;

	//下边界裁剪
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sz == -1) //起点在外侧
		{
			if (codepolypoint[j].sz >= 0)	//外内，输出交点与终点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftDown, RightDown);

				CET* p = new CET;
				CET* q = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
				count++;
			}
		}
		else      //起点在内侧
		{
			if (codepolypoint[j].sz == -1)	//内外，输出交点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftDown, RightDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //内内，输出终点
			{
				CET* q = new CET;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
			}
		}
	}

	count++;
	codepolypoint.clear();
	codepolypoint.resize(count);
	pET = pET->next;

	for (int i = 0; i < count; i++)
	{
		if (pET)
		{
			codepolypoint[i].p.x = pET->x;
			codepolypoint[i].p.y = pET->y;
			codepolypoint[i].sp = spcode(codepolypoint[i].p, LeftUp, RightDown);
			codepolypoint[i].sz = szcode(codepolypoint[i].p, LeftUp, RightDown);
			pET = pET->next;
		}
		else codepolypoint[i] = codepolypoint[0];
	}

	CET* uET = new CET;		//初始化更新边表
	uET->next = NULL;
	pET = uET;
	count = 0;

	//右边界裁剪
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sp == 1) //起点在外侧
		{
			if (codepolypoint[j].sp <= 0)	//外内，输出交点与终点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, RightDown);

				CET* p = new CET;
				CET* q = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
				count++;
			}
		}
		else      //起点在内侧
		{
			if (codepolypoint[j].sp == 1)	//内外，输出交点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, RightDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //内内，输出终点
			{
				CET* q = new CET;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
			}
		}
	}

	count++;
	codepolypoint.clear();
	codepolypoint.resize(count);
	pET = pET->next;
	for (int i = 0; i < count; i++)
	{
		if (pET)
		{
			codepolypoint[i].p.x = pET->x;
			codepolypoint[i].p.y = pET->y;
			codepolypoint[i].sp = spcode(codepolypoint[i].p, LeftUp, RightDown);
			codepolypoint[i].sz = szcode(codepolypoint[i].p, LeftUp, RightDown);
			pET = pET->next;
		}
		else codepolypoint[i] = codepolypoint[0];
	}

	CET* rET = new CET;		//初始化更新边表
	rET->next = NULL;
	pET = rET;
	count = 0;

	//上边界裁剪
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sz == 1) //起点在外侧
		{
			if (codepolypoint[j].sz <= 0)	//外内，输出交点与终点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, LeftUp);

				CET* p = new CET;
				CET* q = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
				count++;
			}
		}
		else      //起点在内侧
		{
			if (codepolypoint[j].sz == 1)	//内外，输出交点
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, LeftUp);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //内内，输出终点
			{
				CET* q = new CET;

				q->x = codepolypoint[j].p.x;
				q->y = codepolypoint[j].p.y;
				q->next = pET->next;

				pET->next = q;
				count++;
			}
		}
	}


	count++;
	codepolypoint.clear();
	codepolypoint.resize(count);
	pET = pET->next;

	for (int i = 0; i < count; i++)
	{
		if (pET)
		{
			codepolypoint[i].p.x = pET->x;
			codepolypoint[i].p.y = pET->y;
			codepolypoint[i].sp = spcode(codepolypoint[i].p, LeftUp, RightDown);
			codepolypoint[i].sz = szcode(codepolypoint[i].p, LeftUp, RightDown);
			pET = pET->next;
		}
		else codepolypoint[i] = codepolypoint[0];
	}

	vector<PixelPoint> rasterPts;
	rasterPts.resize(count - 1);

	for (int i = 0; i < count - 1; i++)
	{
		rasterPts[i].x = codepolypoint[i].p.x;
		rasterPts[i].y = codepolypoint[i].p.y;
	}
	if (rasterPts.size()) 
		Draw::PolyScan(rasterPts.data(), rasterPts.size(),getPenColor());
}