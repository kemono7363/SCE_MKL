#include "scelib.h"


double sce_atan2(double V, double U) {
	double result;
	if (U == 0) {
		if (V == 0)exit(1);
		else result = PI / 2;
	}

	int V_exponent, U_exponent;
	frexp(V, &V_exponent);
	frexp(U, &U_exponent);
	if((V_exponent - U_exponent) > (DBL_MAX_EXP - 3)){
	    //overflow
	    printf("Overflow\n");
	    result = PI / 2;
	    if(V < 0)result = -result;
	}
	else if((V_exponent - U_exponent) < (DBL_MIN_EXP + 3)){
	    //underflow
	    printf("Underflow\n");
	    result = 0;
	    if(U < 0)result = PI - result;
	    if(V < 0)result = -result;
	}
    else{
        double f = fabs(V / U);
        int N = 0;
        if (f > 1) {
            f = (double)1 / f;
            N = 2;
        }
        if (f > (0.26794919243112270647)) {
            const double A     = 0.73205080756887729353;
            const double sqrt3 = 1.73205080756887729353;
            f = (((A*f - 0.5) - 0.5) + f) / (sqrt3 + f);
            N = N + 1;
        }
        if (fabs(f) < DBL_EPSILON) {
            result = f;
        } else {
            double g = f * f;
            const double p0 = -0.13688768894191926929e2;
            const double p1 = -0.20505855195861651981e2;
            const double p2 = -0.84946240351320683534e1;
            const double p3 = -0.83758299368150059274e0;
            const double q0 = 0.41066306682575781263e2;
            const double q1 = 0.86157349597130242515e2;
            const double q2 = 0.59578436142597344465e2;
            const double q3 = 0.15024001160028576121e2;
            const double q4 = 0.10000000000000000000e1;
            double gPg = (((p3*g + p2)*g + p1)*g + p0)*g;
            double Qg = (((g + q3)*g + q2)*g + q1)*g + q0;
            double R = gPg / Qg;
            result = f + f*R;

            if(N > 1)result = -result;

            double a[4];
            a[0] = 0.0;
            a[1] = 0.52359877559829887308;
            a[2] = 1.57079632679489661923;
            a[3] = 1.04719755119659774615;
            result = result + a[N];

            if(U < 0)result = PI - result;
            if(V < 0)result = -result;
        }
    }
	return result;
}

int sce_atan2_test()
{
	double input_V = ((double)(rand()%1000)/100);
	double input_U = ((double)(rand()%1000)/100);
	printf("Input=%f, %f\n", input_V, input_U);
	double my_result = sce_atan2(input_V, input_U);
	double result_comparison = atan2(input_V, input_U);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
		return 1;
	}
	else return 0;
}
