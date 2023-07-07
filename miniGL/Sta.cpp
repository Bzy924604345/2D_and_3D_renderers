#include "Sta.h"
#include "Graphic.h"

Sta g_Sta;

Sta::Sta()
{
	/*DrawMode = dmGrid;*/
	GridColor1 = WHITE;
	GridColor2 = BLUE;
	/*GridWidth = GridHeight = 10;*/

	DrawLineMethod = dlMidBresenham;
	DrawPolygonMethod = dpXScanline;
	Ks = 0.5;
}
