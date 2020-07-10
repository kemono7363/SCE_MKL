
#include <unistd.h> //for sleep(), _POSIX_THREAD_ATTR_STACKSIZE, _POSIX_THREAD_ATTR_STACKADDR, _POSIX_THREAD_PRIORITY_SCHEDULING, getpagesize()
#include <limits.h> //for PTHREAD_STACK_MIN
#include <pthread.h> //for spin lock
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sched.h> //for sched_get_priority_min/max(policy)
#include <math.h> //for rand()
#include <assert.h> //for assert
#include <time.h> //for clock and time related
#include<float.h>
#define PI acos(-1)

//define of cache structure
#define NUM_SERVERS 3
/**
 * INPUT_SHARED_MEMORY_BLOCK_SIZE + OUTPUT_SHARED_MEMORY_BLOCK_SIZE must be no more than
 * PER_CORE_PER_WAY_L1_DATA_CACHE_SIZE_2_POWER * (PER_CORE_NUM_WAYS_L1_DATA_CACHE - 1 (one way reserved for the stack))
 */

#define PER_CORE_PER_WAY_L1_DATA_CACHE_SIZE 4096
#define PER_CORE_NUM_WAYS_L1_DATA_CACHE 8
#define PAGE_SIZE 4096
#define PER_OPERAND_SIZE 1200
#define numOfIntPerOperand 300
#define numOfFloatPerOperand 300
#define numOfDoublePerOperand 150
pthread_attr_t clientThreadAttr;
pthread_t clientThread;
pthread_attr_t serverThreadAttr[NUM_SERVERS];
pthread_t serverThread[NUM_SERVERS];
int serverIndex[NUM_SERVERS]; //serverIndex-th server runs on core (serverIndex+1).

enum CBLAS_ORDER {CblasRowMajor=101, CblasColMajor=102};
enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113};
enum CBLAS_UPLO {CblasUpper=121, CblasLower=122};
enum CBLAS_DIAG {CblasNonUnit=131, CblasUnit=132};
enum CBLAS_SIDE {CblasLeft=141, CblasRight=142};

typedef enum{
	MATRIX_PRODUCT,
	SIN, COS, TAN, COT, ATAN2,
	SQRT, EXP, LOG, ALOG, LOG10, 
	FIND_PRIME_NUMBERS, 
}ServerFunctionType;

typedef struct{
	int length;
	char operand0[PER_OPERAND_SIZE];
	char operand1[PER_OPERAND_SIZE];
	ServerFunctionType functionType;
	pthread_spinlock_t spinlock;
	int jobReady; //either 0 or 1 or -1 serverthreads need to terminate.
}InputSharedMemoryBlock;

typedef enum{
	SUCCESS, FAILURE
}ServerReturnConditionType;

typedef struct{
	char operandResult[PER_OPERAND_SIZE]; //((int*)operandResult)[0], number of found prime number; ((int*)operandResult)[1...], the found prime numbers.
	ServerReturnConditionType returnCondition;
	pthread_spinlock_t spinlock;
	int resultReady; //either 0 or 1 or -1 for terminate server thread.
}OutputSharedMemoryBlock;

typedef struct{
	InputSharedMemoryBlock inputPart;
	OutputSharedMemoryBlock outputPart;
}SharedMemoryBlock;

/** memory aligned for PAGE L1 data cache */
SharedMemoryBlock* pSharedMemoryBlock[NUM_SERVERS];

//declaration of stack allocation
int stack_allocation();  // 1 if successful
//declaration of function
//ref_math
double sce_sin(double f);
double sce_cos(double f);
double sce_tan(double f);
double sce_cot(double f);
double sce_atan2(double V, double U);
double sce_sqrt(double f);
double sce_exp(double f);
double sce_log(double f);
double sce_alog(double f);
double sce_alog10(double f);
double sce_abs(double f);
//ref_blas
double cssce_dot(	
	const int                  N,
	const double               * X,
	const int                  INCX);
double sce_asum(
	const int                  N,
	const double               * X,
	const int                  INCX);
double sce_nrm2(
	const int                  N,
	const double               * X,
	const int                  INCX);
void sce_rot(
	const int                  N,
	double                     * X,
	const int                  INCX,
	double                     * Y,
	const int                  INCY,
	const double               C,
	const double               S);
void sce_rotg(
	double                     * A,
	double                     * B,
	double                     * C,
	double                     * S);
void sce_rotm(
	const int                  N,
	double                     * X,
	const int                  INCX,
	double                     * Y,
	const int                  INCY,
	const double               * PARAM);
void sce_rotmg(
	double                     * D1,
	double                     * D2,
	double                     * X1,
	const double Y1,
	double                     * PARAM);
void sce_scal(
	const int                  N,
	const double               ALPHA,
	double                     * X,
	const int                  INCX);
void sce_swap(
	const int                  N,
	double                     * X,
	const int                  INCX,
	double                     * Y,
	const int                  INCY);
void sce_axpy(
	const int                  N,
	const double               ALPHA,
	const double               * X,
	const int                  INCX,
	double                     * Y,
	const int                  INCY);
