#include "scelib.h"
void sce_axpy
(
   const int                  N,
   const double               ALPHA,
   const double               * X,
   const int                  INCX,
   double                     * Y,
   const int                  INCY
)
{
	double alpha=ALPHA;
	double x,y;
	int i;
	for( i = 0; i != N; i++ ){
		x  = (*X);
		y  = (*Y);
		*Y  = y + alpha * x;
		X  += INCX;
		Y  += INCY;
	}
}