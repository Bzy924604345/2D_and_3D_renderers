/********************************************************************
file base:	Graphic
file ext:	h
author:		����(njnu2016@foxmail.com)
purpose:	ͼ�ο�ӿ�
version:    1.0
*********************************************************************/
#ifndef _GRAPH_H
#define _GRAPH_H

#define BLACK 0
#define WHITE 0xFFFFFF
#define RED 0xFF
#define GREEN 0xFF00
#define BLUE 0xFF0000
#define YELLOW 0x00FFFF
#define ORANGE   0x1E90FF 
#define CYAN 0xF5F5DC 
#define PURPLE 0x800080

//#define  DIRECT_DRAW

typedef unsigned Color;
typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int DWord;

#pragma region rgb

///����r,g,b����ֵ������ɫ������Ҫ��r,g,bΪ[0,255]�ڵ�����
#define _RGB(r,g,b)  ((unsigned)(((Byte)(r)|((WORD)((Byte)(g))<<8))|(((DWord)(Byte)(b))<<16)))

///����r,g,b,a����ֵ������ɫ������Ҫ��r,g,b,aΪ[0,255]�ڵ�����
#define _RGBA(r,g,b,a) ((unsigned)(((Byte)(r)|((WORD)((Byte)(g))<<8))|(((DWord)(Byte)(b))<<16))|(((DWord)(Byte)(a))<<24))

///����ɫ������ȡr,g,b,a����
#define  _R( color ) ((Byte)(color))
#define  _G( color ) ((Byte)(color >> 8))
#define  _B( color ) ((Byte)(color >> 16))
#define  _A( color ) ((Byte)(color >> 24))

#pragma endregion 

///���ص�
struct PixelPoint
{
	int x, y;
};

/**	��ʼ��
@return �Ƿ��ʼ���ɹ���0-success�� -1 - fail
*/
int init();

/**	��ʼ��
@return �Ƿ��ʼ���ɹ���0-success�� -1 - fail
*/
int init(unsigned hwnd);

#pragma region window_info
/**	��ȡ���ڿ��
@return ���ڿ��
*/
int getWindowWidth();

/**	��ȡ���ڸ߶�
@return ���ڸ߶�
*/
int getWindowHeight();

/**	��ȡ����ʵ�ʴ�С����mmΪ��λ
*/
double getPixelSize();

#pragma endregion 

#pragma region keyboard

/**	�ж�Shift���Ƿ���
@return  Shift���Ƿ���
*/
bool isShiftKeyPressed();

/**	�ж�Ctrl���Ƿ���
@return  Ctrl���Ƿ���
*/
bool isCtrlKeyPressed();

/**	�ж�Alt���Ƿ���
@return  Alt���Ƿ���
*/
bool isAltKeyPressed();

/**	�ж�Caps���Ƿ���
@return  Caps���Ƿ���
*/
bool isCapsKeyPressed();

#pragma endregion 

#pragma region drawing

/**	����
@param  x0 ���x����
@param  y0 ���y����
@param  x1 �յ�x����
@param  y1 �յ�y����
*/
void drawLine(int x0, int y0, int x1, int y1);

/**	�������
@param  pts ����ζ��㼯��
@param  count  ����ζ�����
*/
void drawPolygon(PixelPoint* pts, int count);

/**	ˢ�´���
*/
void refreshWindow();

/**	����ָ���߼�λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setPixel(int x, int y, Color color);

/**	����ָ���߼�λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setPixel(int x, int y, float z, Color color);

/**	����ָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setDevicePixel(int x, int y, Color color);

/**	����ָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@param color ��ɫ
*/
void setDevicePixel(int x, int y, float z, Color color);

/**	��ȡָ��λ�����ص���ɫ
@param  x ����x����
@param  y  ����y����
@return ָ��λ�����ص���ɫ
*/
Color getPixel(int x, int y);

/**	��ȡָ������λ�����ص���ɫ������ϵԭ��λ�����Ͻǣ�x���ң�y����
@param  x ����x����
@param  y  ����y����
@return ָ��λ�����ص���ɫ
*/
Color getDevicePixel(int x, int y);

/**	��ȡ����ɫ
*/
Color getBackColor();

/**	���ñ���ɫ
@param  color ����ɫ
*/
void setBackColor(Color color);

/**	��ȡ������ɫ
*/
Color getPenColor();

/**	���û���ɫ
@param  color ����ɫ
*/
void setPenColor(Color color);

#pragma endregion 

#pragma region coordSys
/**	����Y�᷽��
@param  isUp true��ʾ���ϣ���������
*/
void setYUp(bool isUp);

/**	��ȡ�߼�����ϵY�᷽��
@return Y���Ƿ�����
*/
bool isYUp();

/**	�����߼�����ϵԭ��Ϊָ������λ��
@param  x ָ������λ��x����
@param  y ָ������λ��y����
*/
void setOrig(int x, int y);

/**	��ȡ�߼�����ϵԭ������λ��
@param  x ԭ��x����
@param  y ԭ��y����
*/
void getOrig(int& x, int& y);

/**	�豸����תΪ�߼�����
@param  dx �豸x����
@param  dy  �豸y����
@param  lx �߼�x����
@param  ly  �߼�y����
*/
void DPtToLPt(int dx, int dy, int& lx, int& ly);

/**	�߼�����תΪ�豸����
@param  lx �߼�x����
@param  ly  �߼�y����
@param  dx �豸x����
@param  dy  �豸y����
*/
void LPtToDPt(int lx, int ly, int& dx, int& dy);

#pragma endregion 

#pragma region rubberLine

///�����ʽ��csArrow--��ͷ ��  csHand--���ͣ� csCross--ʮ��˿�� csSize--����
enum CursorStyle { csArrow = 32512, csHand = 32649, csCross = 32515, csSize = 32646 };

/**	���ù����ʽ
@param  cursor �����ʽ
*/
void setCursor(CursorStyle cursor);

///��Ƥ��ģʽ, rmNone -- �ر���Ƥ��ģʽ ��rmLine-- ��ģʽ�� rmRectangle -- ����ģʽ , rmPolyline-- ����ģʽ, rmPolygon -- �����ģʽ
enum RubberMode { rmNone, rmLine, rmRectangle, rmPolyline, rmPolygon };

/**	���õ�ǰģʽ�Ƿ�Ϊ��Ƥ��ģʽ
@param  value �Ƿ���Ƥ��ģʽ
*/
void setRubberMode(RubberMode mode);

/**	��ȡ��ǰ��Ƥ��ģʽ
@return ��ǰ��Ƥ��ģʽ
*/
RubberMode getRubberMode();

/**	��ȡ��Ƥ�ߵ�������һ��������ǰ����ռ�
@return ��Ƥ�ߵ���
*/
int getRubberPointCount();

/**	��ȡ��Ƥ�������յ㣬��������������Ƥ��ģʽ����������Ƥ��
@param  x1 ���x����
@param  y1  ���y����
@param  x2 �յ�x����
@param  y2  �յ�y����
@return ��Ч��Ƥ�ߵ���
*/
int getRubberPoints(int& x1, int& y1, int& x2, int& y2);

/**	��ȡ��Ƥ�������յ㣬��������������Ƥ��ģʽ����������Ƥ��
@param  pt1 ���
@param  pt2 �յ�
@return ��Ч��Ƥ�ߵ���
*/
int getRubberPoints(PixelPoint& pt1, PixelPoint& pt2);

/**	��ȡ��Ƥ�ߵ㼯�ϣ�һ������rmPolyline��rmPolygonģʽ
@param  pts ��Ƥ�ߵ㼯��
@return ��Ƥ�ߵ���
*/
int getRubberPoints(PixelPoint pts[]);
#pragma endregion 

#pragma region font

/**	��ȡ��ǰ�����С
@return ���������С
*/
int getFontSize();

/**	���������С
@param  fontSize �����С
*/
void setFontSize(int fontSize);

/**	�����Сת��Ϊ����߶�
@param  fontSize �����С
@return ��������߶�
*/
int fontSizeToFontHeight(int fontSize);

/**	����߶�ת��Ϊ�����С
@param  fontHeight ����߶�
@return ���������С
*/
int fontHeightToFontSize(int fontHeight);

/**	��ȡ������
@return ����������
*/
const char* getFontName();

/**	����������
@param  fontName ������
*/
void setFontName(const char* fontName);

/**	��ȡ���ֽ��ַ�λͼ
@remark λͼ����x����Ϊ����y����Ϊ����ʵ��λͼ����ֻռ�ַ�����һ���֣�ͨ��offset_x��offset_y��gryph_width��gryph_height��ͬȷ��λͼ��Χ
@param  ch ���ֽ��ַ�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@param  cell_width �ַ������
@param  cell_width �ַ����߶�
@return ���ص��ַ�λͼ������
*/
Byte* getCharGlyph(char ch, int& offset_x, int& offset_y, int& gryph_width, int& gryph_height, int& cell_width, int& cell_height);

/**	��ȡ˫�ֽ��ַ�λͼ
@remark λͼ����x����Ϊ����y����Ϊ����ʵ��λͼ����ֻռ�ַ�����һ���֣�ͨ��offset_x��offset_y��gryph_width��gryph_height��ͬȷ��λͼ��Χ
@param  ch ˫�ֽ��ַ�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@param  cell_width �ַ������
@param  cell_width �ַ����߶�
@return ���ص��ַ�λͼ������
*/
Byte* getWCharGlyph(wchar_t ch, int& offset_x, int& offset_y, int& gryph_width, int& gryph_height, int& cell_width, int& cell_height);

/**	��ȡ�ַ�λͼ����ֵ 0 or 1
@remark �ַ�λͼ����ֵ = 0 ��ʾ�ַ�������ͨ����λ�ã�= 1 ��ʾ�ַ�����ͨ����λ��
@param  pCharGlyph �ַ�λͼ
@param  x ����ˮƽλ�ã�������ַ������߿�
@param  y ���ش�ֱλ�ã�������ַ���򶥱߿�
@param  offset_x �ַ�λͼ��������ַ�������Ͻ�x����ƫ��
@param  offset_y �ַ�λͼ��������ַ�������Ͻ�y����ƫ�ƣ�����Ϊ��
@param  gryph_width �ַ�λͼ�����
@param  gryph_height �ַ�λͼ������
@return ����0 or 1
*/
Byte getCharGlyphPixel(Byte* pCharGlyph, int x, int y, int offset_x, int offset_y, int gryph_width, int gryph_height);

#pragma endregion 

#endif