#include "scelib.h"

double sce_exp(double X) {
	if (X > log(DBL_MAX))exit(1);
	else if (X < log(DBL_MIN))exit(1);

	double result;
	if (fabs(X) < DBL_EPSILON) {
		result = 1.0;
	}
	else {
		int N = round(X * 1.4426950408889634074);
		double XN = (double)N;
		const double C1 = (double)355 / 512;
		const double C2 = -2.1219444005469058277e-4;
		double X1 = (double)((int)X);
		double X2 = X - X1;
		double g = ((X1 - XN * C1) + X2) - XN * C2;
		double z = g * g;
		const double p0 = 0.249999999999999993e0;
		const double p1 = 0.694360001511792852e-2;
		const double p2 = 0.165203300268279130e-4;
		const double q0 = 0.500000000000000000e0;
		const double q1 = 0.555538666969001118e-1;
		const double q2 = 0.495862884905441294e-3;
		double gPz = ((p2*z + p1)*z + p0)*g;
		double Qz = (q2*z + q1)*z + q0;
		double Rg = .5 + gPz / (Qz - gPz);
		result = Rg;
		for (int i = 0; i <= N; i++) {
			result *= 2;
		}
	}
	return result;
}


int sce_exp_test()
{
	double input = ((double)(rand()%1000)/100);
	printf("Input=%f\n", input);
	double my_result = sce_exp(input);
	double result_comparison = exp(input);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
		return 1;
	}
	else return 0;
}
