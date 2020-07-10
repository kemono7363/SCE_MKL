#include "scelib.h"
void sce_rotg
(
	double                     * A,
	double                     * B,
	double                     * C,
	double                     * S
)
{
	
	
	double absa, absb, roe, scale, r, tmpa, tmpb, z;
	absa = sce_abs( *A ); absb = sce_abs( *B );
	roe  = ( absa > absb ? (*A) :  (*B) ); scale = absa + absb;

	if( scale != 0 )
	{
	tmpa = (*A) / scale; tmpb = (*B) / scale;
	if( roe < 0 )
	{ r = - scale * sce_sqrt( ( tmpa * tmpa ) + ( tmpb * tmpb ) ); }
	else
	{ r =   scale * sce_sqrt( ( tmpa * tmpa ) + ( tmpb * tmpb ) ); }
	*C = (*A) / r; *S = (*B) / r; z =  1;
	if(   absa >  absb ) { z = *S; }
	if( ( absb >= absa ) && ( (*C) != 0 ) )
	{ z = 1 / (*C); }
	}
	else { *C = 1; *S = r = z = 0; }

	*A = r; *B = z;
}