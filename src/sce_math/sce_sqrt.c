#include "scelib.h"

// float: [1, 8, 23][sum=32]
//double: [1, 11, 52][sum=64]

double sce_sqrt(double X0)
{
	double X, f;
	int N;
	if(X0 == 0.0)return X0;
	else if(X0 < 0.0){
	    printf("X<0 error!\n");
	    X = fabs(X0);
	}
	else X = X0;
	f = frexp(X, &N);
	double y = 0.41731 + 0.59016 * f;
	/*
	for(int j=0;j<3;j++)	{
	    y = .5 * (y + f/y);
	}
	*/
    double z = y + f/y;
    y = ldexp(z, -2) + f/z;
    y = ldexp((y+f/y), -1);
    if(N%2!=0){
        y *= 0.70710678118654752440;
        N++;
    }
    int M = N >> 1;
    double result = ldexp(y, M);
    return result;
}

int sce_sqrt_test()
{
	double input = ((double)(rand()%1000)/100);
	printf("Input=%f\n", input);
	double my_result = sce_sqrt(input);
	double result_comparison = sqrt(input);
	printf("Correct: %lf\n", result_comparison);
	printf("My result: %lf\n", my_result);
	if (fabs(my_result - result_comparison) < DBL_EPSILON) {
        printf("Equal\n");
		return 1;
	}
	else{
	    printf("Not Equal\n");
	    return 0;
	}
}
