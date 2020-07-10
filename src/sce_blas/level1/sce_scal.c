#include "scelib.h"
void sce_scal
(
   const int                  N,
   const double               ALPHA,
   double                     * X,
   const int                  INCX
)
{
	double x;
	double alpha=ALPHA;
	int i;
	for( i = 0; i != N; i++ ){
		x  = (*X);
		x *= alpha;
		*X  = x;
		X += INCX;
	}
}