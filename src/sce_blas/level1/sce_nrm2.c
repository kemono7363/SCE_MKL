#include "scelib.h"
double sce_nrm2
(
   const int                  N,
   const double               * X,
   const int                  INCX
)
{
	
	double absxi,scale=0,ssq=1,temp=0;
	double x;
	int i;
	
	for( i = 0; i != N; i++ )
	{
		x  = (*X);

		absxi = sce_abs( x );
		if( scale < absxi ){
		temp = scale / absxi; scale = absxi;
		ssq  = 1 + ssq * ( temp * temp );
		}
		else { temp = absxi / scale; ssq += temp * temp; }
		X  += INCX;
	}
	return( scale * sce_sqrt( ssq ) );
}