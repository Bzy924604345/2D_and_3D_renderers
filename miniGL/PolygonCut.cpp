#include <iostream>
#include <windows.h>
#include "Graphic.h"
#include "Draw.h"
#include "Sta.h"
#include <vector>

using namespace std;

// �ü�������ṹ��
typedef struct CET
{
	float x;
	float y;
	CET* next;
};

// ������ṹ�壬��¼�������Լ�ˮƽ��ֱ����
typedef struct codePixelPoint
{
	PixelPoint p;
	int sp;
	int sz;
};

//ˮƽ���뺯��,��-1����0����1
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

//��ֱ���뺯������-1����0����1
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

// �󽻵�
PixelPoint Draw::jiaodian(PixelPoint firstpoint, PixelPoint secondpoint, PixelPoint s1, PixelPoint s2)
{
	PixelPoint jiaodian;
	double k;
	if (s1.x == s2.x) //��ֱ��
	{
		if (firstpoint.x == secondpoint.x)  // ͬΪ��ֱ��
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


	if (s1.y == s2.y) // ˮƽ��
	{
		if (firstpoint.y == secondpoint.y) // ͬΪˮƽ��
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
	// ȷ���ü���Χ
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

	//����
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

	int count = 0;//�ü������
	CET* pET = new CET;	//��ʼ���ü��߱�
	pET->next = NULL;

	//��߽�ü�
	for (int i = 0, j = 1; i < size - 1; i++, j++)
	{
		if (codepolypoint[i].sp == -1) //��������
		{
			if (codepolypoint[j].sp >= 0)	//���ڣ�����������յ�
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
			// ���ⲻ���
		}
		else      //������ڲ�
		{
			if (codepolypoint[j].sp == -1)	//���⣬�������
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftUp, LeftDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //���ڣ�����յ�
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
	// ���ñ������
	codepolypoint.clear();
	codepolypoint.resize(count);
	pET = pET->next; // ָ�����һ����


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



	CET* lET = new CET;		//��ʼ�����±߱�
	lET->next = NULL;
	pET = lET;
	count = 0;

	//�±߽�ü�
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sz == -1) //��������
		{
			if (codepolypoint[j].sz >= 0)	//���ڣ�����������յ�
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
		else      //������ڲ�
		{
			if (codepolypoint[j].sz == -1)	//���⣬�������
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, LeftDown, RightDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //���ڣ�����յ�
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

	CET* uET = new CET;		//��ʼ�����±߱�
	uET->next = NULL;
	pET = uET;
	count = 0;

	//�ұ߽�ü�
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sp == 1) //��������
		{
			if (codepolypoint[j].sp <= 0)	//���ڣ�����������յ�
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
		else      //������ڲ�
		{
			if (codepolypoint[j].sp == 1)	//���⣬�������
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, RightDown);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //���ڣ�����յ�
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

	CET* rET = new CET;		//��ʼ�����±߱�
	rET->next = NULL;
	pET = rET;
	count = 0;

	//�ϱ߽�ü�
	for (int i = 0, j = 1; i < codepolypoint.size() - 1; i++, j++)
	{
		if (codepolypoint[i].sz == 1) //��������
		{
			if (codepolypoint[j].sz <= 0)	//���ڣ�����������յ�
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
		else      //������ڲ�
		{
			if (codepolypoint[j].sz == 1)	//���⣬�������
			{
				PixelPoint point = jiaodian(codepolypoint[i].p, codepolypoint[j].p, RightUp, LeftUp);
				CET* p = new CET;

				p->x = point.x;
				p->y = point.y;
				p->next = pET->next;

				pET->next = p;
				count++;
			}
			else   //���ڣ�����յ�
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