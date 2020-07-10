#include "scelib.h"
// float: [1, 8, 23][sum=32]
//double: [1, 11, 52][sum=64]

double sce_tan(double X)
{
	double Y = fabs(X);
	//printf("Y=%f\n", Y);
	//YMAX=PI*B**(t/2), assert if Y>YMAX, YMAX is the integer part of B**(t/2)*pi/2
	int N = round(X * 2 / PI);
	double XN = (double)abs(N);
	double X1 = (double)((int)Y);
	double X2 = Y - X1;
	const double C1 = (double)3217 / 2048;
	const double C2 = -4.454455103380768678308e-6;
	//printf("N=%d, X1=%f, X2=%f, C1=%f, C2=%f\n", N, X1, X2, C1, C2);
	double f = ((X1 - XN * C1) + X2) - XN * C2;
	//printf("f=%f\n", f);
	double XNUM, XDEN;
	if(fabs(f) < DBL_EPSILON){
        XNUM = f;
        XDEN = 1.0;
    }
    else{
        double g = f * f;
        const double p0 = 0.10000000000000000000e1;
        const double p1 = -0.1282834704095743847e0;
        const double p2 = 0.2805918241169988906e-2;
        const double p3 = -0.7483634966612065149e-5;
        const double q0 = 0.10000000000000000000e1;
        const double q1 = -0.4616168037429048840e0;
        const double q2 = 0.2334485282206872802e-1;
        const double q3 = -0.2084480442203870948e-3;
        double fPg = ((p3*g + p2)*g + p1)*g*f + f;
        double Qg = ((q3*g + q2)*g + q1)*g + q0;
        XNUM = fPg;
        XDEN = Qg;
    }
	if (N % 2 == 0) {
		return (XNUM / XDEN);
	}
	else {
		return (-XDEN / XNUM);
	}
}

int sce_tan_test()
{
	double input = ((double)(rand()%1000)/100);
	printf("Input=%f\n", input);
	double my_result = sce_tan(input);
	double result_comparison = tan(input);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
		return 1;
	}
	else return 0;
}
