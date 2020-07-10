#include "scelib.h"

//Referencing: SOFTWARE MANUAL FOR THE ELEMENTARY FUNCTIONS by William J.Cody and William Waite, P35-59

double sce_log(double X) {
	if (X <= 0.0)exit(1);
	int N;
	double f = frexp(X, &N);
	//printf("f=%f\n", f);
	//printf("N=%d\n", N);
	double znum, zden;
	if (f > 0.70710678118654752440) {
		znum = (f - .5) - .5;
		zden = f * .5 + .5;
	}
	else {
		N = N - 1;
		znum = f - .5;
		zden = znum * .5 + .5;
	}
	//printf("znum=%f\n", znum);
	//printf("zden=%f\n", zden);
	double z = znum / zden;
	double w = z*z;
	const double a0 = -0.64124943423745581147e2;
	const double a1 = 0.16383943563021534222e2;
	const double a2 = -0.78956112887491257267e0;
	const double b0 = -0.76749932108494879777e3;
	const double b1 = 0.31203222091924532844e3;
	const double b2 = -0.35667977739034646171e2;
	const double b3 = 0.10000000000000000000e1;
	double Bw = ((w + b2)*w + b1)*w + b0;
	double Aw = ((w + a2)*w + a1)*w + a0;
	double rz2 = w * Aw / Bw;
	double Rz = z + z * rz2;
	//printf("Rz=%f\n", Rz);
	const double C1 = (double)355 / 512;
	const double C2 = -2.121944400546905827679e-4;
	double XN = (double)N;
	double result = (XN*C2 + Rz) + XN * C1;
	return result;
}

int sce_log_test()
{
	double input = ((double)(rand()%1000)/100);
	printf("Input=%f\n", input);
	double my_result = sce_log(input);
	double result_comparison = log(input);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
		return 1;
	}
	else return 0;
}
