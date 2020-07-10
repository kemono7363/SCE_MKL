#include "scelib.h"
double sce_dot
(
	const int                  N,
	const double               * X,
	const int                  INCX,
	const double               * Y,
	const int                  INCY
)
{

	double dot = 0,x,y;
	int i;
	for (i=0;i<N;i++){
		x=(*X);
		y=(*Y);
		dot += x*y;
		X+=INCX;
		Y+=INCY;

	}
	return dot;

}