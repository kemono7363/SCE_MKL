#include "scelib.h"

// float: [1, 8, 23][sum=32]
//double: [1, 11, 52][sum=64]

double sce_sin(double X)
{
	double SGN, Y;
	if (X < 0.0)
	{
		SGN = -1;
		Y = -X;
	}
	else
	{
		SGN = 1;
		Y = X;
	}
	//YMAX=PI*B**(t/2), assert if Y>YMAX
	int N = round(Y / PI);
	double XN = (double)N;

	if (N % 2 == 1)SGN = -SGN;
	//pseudo extended precision computation
	double X1 = (double)((int)Y);
	double X2 = Y - X1;
	//printf("X1=%f, X2=%f\n", X1, X2);
	const double C1 = (double)3217 / 1024;
	const double C2 = -8.908910206761537356617e-6;
	double f = ((X1 - XN * C1) + X2) - XN * C2;
	if (fabs(f) < DBL_EPSILON)return f;
	double g = f * f;

	double r1 = -0.16666666666666665052;//-1/(3!)
	double r2 =  0.83333333333331650314e-2;
	double r3 = -0.19841269841201840457e-3;
	double r4 =  0.27557319210152756119e-5;
	double r5 = -0.25052106798274584544e-7;
	double r6 =  0.16058936490371589114e-9;
	double r7 = -0.76429178068910467734e-12;
	double r8 = 0.27204790957888846175e-14;
	double Rg = (((((((r8*g + r7)*g + r6)*g + r5)*g + r4)*g + r3)*g + r2)*g + r1)*g;
	//double Rg = (((r4*g + r3)*g + r2)*g + r1)*g;
	double result = f + f * Rg;
	result *= SGN;
	return result;
}

int sce_sin_test()
{
	double input = ((double)(rand()%1000)/100);
	printf("Input=%f\n", input);
	double my_result = sce_sin(input);
	double result_comparison = sin(input);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
		return 1;
	}
	else return 0;
}
