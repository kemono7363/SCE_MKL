#include "scelib.h"
double sce_asum
(
	const int                  N,
	const double               * X,
	const int                  INCX
)
{
	double sum =0,x;
	int i;
	for (i=0;i<N;i++){
		x=(*X);
		sum += sce_abs(x);
		X+=INCX;

	}
	return sum;

}