#include <stdio.h>
#include<float.h>
#include "scelib.h"

int main()
{
	/* first round of test in sce_math
	SIN, COS, TAN, COT, ATAN2,
	SQRT, EXP, LOG, ALOG, LOG10,
	*/
	printf("PI value:%f\n", PI);
	//printf("Input \t sce_sin_result \t math_sin_result\n");
	int testTime=10000;
	int flag=1;
	double sin_,answer_sin;
	double cos_,answer_cos;
	double r;
	for(int i=0;i<testTime;i++){
	    r = (double)(rand()%1000000)/1000000;
	    sin_ =sce_sin(r*PI);
	    answer_sin =sin(r*PI);
		if (sin_!=answer_sin) {
			printf("%d\n",i);
			flag=0;
			}
	    //printf("%d\t%lfPI \t %lf \t %lf\n", i,r, sin_, answer_sin);
	}
    if (flag){
		printf("sin test successfully by passing %d random tests",testTime);
	}else{
		printf("calculation error");
	}
	printf("\n");
	flag=1;
	//printf("Input \t sce_cos_result \t math_cos_result\n");
	for(int i=0;i<testTime;i++){
	    r = (double)(rand()%1000000)/1000000;
	    cos_ =sce_cos(r*PI);
	    answer_cos =cos(r*PI);
		if (cos_!=answer_cos) {
			printf("%d",i);
			flag=0;
			}
	    //printf("%d\t%lfPI \t %lf \t %lf\n", i,r, cos_, answer_cos);
	}
	if (flag){
		printf("cos test successfully by passing %d random tests",testTime);
	}else{
		printf("calculation error");
	}
        printf("\n");
	return 0;
}
