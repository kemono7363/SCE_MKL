#include "scelib.h"
void sce_swap
(
   const int                  N,
   double                     * X,
   const int                  INCX,
   double                     * Y,
   const int                  INCY
)
{
	double x,y;
	int i;
	for( i = 0; i != N; i++ ){
		x  = (*X);
		y  = (*Y);
		*Y  = x;
		*X  = y;
		X  += INCX;
		Y  += INCY;
	}
}