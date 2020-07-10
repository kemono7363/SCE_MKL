#include "scelib.h"
// float: [1, 8, 23][sum=32]
//double: [1, 11, 52][sum=64]

double sce_abs(double X)
{
	if(X<0.0)return -X;
	else return X;
}
