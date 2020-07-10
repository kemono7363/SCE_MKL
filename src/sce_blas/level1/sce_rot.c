#include "scelib.h"
void sce_rot
(
	const int                  N,
	double                     * X,
	const int                  INCX,
	double                     * Y,
	const int                  INCY,
	const double               C,
	const double               S
)
{
	double x,y;
	double co=C,si=S;
	int i;
	for( i = 0; i != N; i++ ){
		x  = (*X);
		y  = (*Y);

		*X  = co * x + si * y;
		*Y  = co * y - si * x;

		X  += INCX;
		Y  += INCY;
	}
}