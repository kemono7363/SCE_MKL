#include "scelib.h"

#define   GAM	4096.0
#define   GAMSQ 16777216.0
void sce_rotmg
(
	double                     * D1,
	double                     * D2,
	double                     * X1,
	const double Y1,
	double                     * PARAM
)
{
	double	gam	= GAM, rgam = 1 / GAM;
	double	gamsq	= GAMSQ, rgamsq = 1 / GAMSQ;
	double  d1 = (*D1), d2 = (*D2), flag, h11, h12, h21, h22,
            p1, p2, q1, q2, tmp, u, x1 = (*X1);
	h11 = h12 = h21 = h22 = 0;

	if ( d1 < 0 )
	{
		PARAM[0]	= -1;
		PARAM[1]	= PARAM[2] = PARAM[3] = PARAM[4] = 0;
		*D1		= *D2 = *X1 = 0;
		return;
	}

	if ( (p2 = d2 * Y1) == 0 )
	{
		PARAM[0] = -2; return;
	}

	p1 = d1 * x1; q2 = p2 * Y1; q1 = p1 * x1;

	if ( sce_abs( q1 ) > sce_abs( q2 ) )
	{
		h21 = -Y1 / x1; h12 = p2 / p1; u = 1 - h12 * h21;

		if ( u <= 0 )
		{
			PARAM[0]	= -1;
			PARAM[1]	= PARAM[2] = PARAM[3] = PARAM[4] = 0;
			*D1		= *D2 = *X1 = 0;
			return;
		}
		flag = 0; d1 /= u; d2 /= u; x1 *= u;
	}else  {
		if ( q2 < 0 )
		{
			PARAM[0]	= -1;
			PARAM[1]	= PARAM[2] = PARAM[3] = PARAM[4] = 0;
			*D1		= *D2 = *X1 = 0;
			return;
		}
		flag	= 1;
		h11	= p1 / p2; h22 = x1 / Y1;
		u	= 1 + h11 * h22; tmp = d2 / u;
		d2	= d1 / u; d1 = tmp; x1 = Y1 * u;
	}

	if ( d1 <= rgamsq )
	{
		if ( d1 != 0 )       /* scale d1 up */
		{
			if ( flag == 0 )
			{
				h11 = 1;  h22 = 1; flag = -1;
			}else if ( flag > 0 )
			{
				h21 = -1; h12 = 1; flag = -1;
			}

			do
			{
				d1 *= gamsq; x1 *= rgam; h11 *= rgam; h12 *= rgam;
			}
			while ( d1 <= rgamsq );
		}
	}else if ( d1 >= gamsq )                  /* scale d1 down */
	{
		if ( flag == 0 )
		{
			h11 = 1;  h22 = 1; flag = -1;
		}else if ( flag > 0 )
		{
			h21 = -1; h12 = 1; flag = -1;
		}

		do
		{
			d1 *= rgamsq; x1 *= gam; h11 *= gam; h12 *= gam;
		}
		while ( d1 >= gamsq );
	}

	if ( (tmp = sce_abs( d2 ) ) <= rgamsq )
	{
		if ( d2 != 0 )     /* scale d2 up */
		{
			if ( flag == 0 )
			{
				h11 = 1;  h22 = 1; flag = -1;
			}else if ( flag > 0 )
			{
				h21 = -1; h12 = 1; flag = -1;
			}

			if ( d2 > 0 )
			{
				do
				{
					d2 *= gamsq; h21 *= rgam; h22 *= rgam;
				}
				while ( d2 <= rgamsq );
			}else  {
				do
				{
					d2 *= gamsq; h21 *= rgam; h22 *= rgam;
				}
				while ( d2 >= -rgamsq );
			}
		}
	}else if ( tmp >= gamsq )               /* scale d2 down */
	{
		if ( flag == 0 )
		{
			h11 = 1;  h22 = 1; flag = -1;
		}else if ( flag > 0 )
		{
			h21 = -1; h12 = 1; flag = -1;
		}

		if ( d2 > 0 )
		{
			do
			{
				d2 *= rgamsq; h21 *= gam; h22 *= gam;
			}
			while ( d2 >= gamsq );
		}else  {
			do
			{
				d2 *= rgamsq; h21 *= gam; h22 *= gam;
			}
			while ( d2 <= -gamsq );
		}
	}

	*D1 = d1; *D2 = d2; *X1 = x1;

	PARAM[0] = flag;
	if ( flag < 0 )
	{
		PARAM[1] = h11; PARAM[2] = h21; PARAM[3] = h12; PARAM[4] = h22;
	}else if ( flag == 0 )
	{
		PARAM[2] = h21; PARAM[3] = h12;
	}else { PARAM[1] = h11; PARAM[4] = h22; }
}
