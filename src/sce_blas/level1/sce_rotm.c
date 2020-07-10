#include "scelib.h"
void sce_rotm
(
   const int                  N,
   double                     * X,
   const int                  INCX,
   double                     * Y,
   const int                  INCY,
   const double               * PARAM
)
{
	double h11,h12,h21,h22;
	double x,y;
	int i;
	if( ( N > 0 ) && ( *PARAM != -2 ) )
	{
	if(      *PARAM   == -1 ){
		h11 = PARAM[1]; h21 = PARAM[2]; h12 = PARAM[3]; h22 = PARAM[4];
		for( i = 0; i != N; i++ ){
			x  = (*X); y  = (*Y);
			*X  = ( x * h11 ) + ( y * h12 );
			*Y  = ( x * h21 ) + ( y * h22 );
			X  += INCX;
			Y  += INCY;
		}
	}
	else if( *PARAM == 0 ){
		h21 = PARAM[2]; h12 = PARAM[3];
		for( i = 0; i != N; i++ ){
			x  = (*X); y  = (*Y);
			*X  = x + ( y * h12 ); *Y  = y + ( x * h21 );
			X  += INCX; Y  += INCY;
		}
	}
	else if( *PARAM == 1 ){
		h11 = PARAM[1]; h22 = PARAM[4];
		for( i = 0; i != N; i++ ){
			x  = (*X); y = (*Y);
			*X  = ( x * h11 ) + y; *Y  = ( h22 * y ) - x;
			X  += INCX; Y  += INCY;
		 }
	  }
	}
}