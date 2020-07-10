#include <stdlib.h>
#include "scelib.h"
#include "scemisc.h"
#include "debug.h"
#include<float.h>

/*
variable explanation: assume that 3 server threads(4 in total threads) are used in one dispatch iteration
xxx-> replaced by name of function
iteration -> outer loop of client dispatch
dispatch -> inner loop of client dispatch
outer iteration loop:
	numOfIteration_sin_xxx: total times needed to dsipatch subtask according to the need of the task
		dispatchTimes(inner loop defined variable): current times defined by times that needed to dispatch subtask according to the need of the task. 
				eg. dot_product of two vector, each of which has 8100 numbers, 
				for each server loop, dot product of 2 product (300 numbers included) 
				2 vectors containing 900 numbers of each will be processed
				dispatch_times = 8100 / 900 = 9	

inner dispatch loop:
	NUM_SERVERS predefined in .h file, the number of server thread
	numOfDispatch_sin : times needed for dispatch under the environment of the task
		serverIndex(inner loop defined variable): in the current dispatch process, order of server thread that accepts data
				eg. two vectors, each has 900 numbers dispatched in one iteration
				serverIndex =0 accept data[0:299]
				serverIndex =1 accept data[300:599]
				serverIndex =2 accept data[600:899]
parameter to transfer:
	function:	
		variable defined in order to simplify parameters transfer.
	list:	
		numOfPara_xxx	the total number of parameters needed to pass from client to server 
				eg. sce_sin(x) only 1 variable needed to pass
		numOfParaI_xxx	the total number of parameters needed to pass per iteration
		numOfparaD_xxx	the total number of parameters needed to pass per dispatch 
		input_xxx[]		local global input of parameter 
 		output_xxx[]		local global output of the result
		numOfIntPerOperand 300
		numOfFloatPerOperand 300
		numOfFloatPerOperand 150
*/

//parameter variable
//predefined suitable for function
int numOfIteration_sin=1; //the number of outer loop needed in this function
int numOfDispatch_sin=1; //the number of inner loop needed in this function
int numOfPara_sin=1;	 //the total number of parameters needed to pass from client to server 
int numOfParaI_sin=1;	 //the total number of parameters needed to pass per iteration
int numOfParaD_sin=1;    //the total number of parameters needed to pass per dispatch 
//param container
double input_sin=0;		 //input global container
double output_sin=0;		 //output global container	

/*procedure to revise 
1.parameter datatypes, mostly double 1200/8=150 for each block
2.input parameters initialization
3._xxx function name
4.numOf___ 5 parameters
5.name of thread function including pthread_create eg. clientMain_xxx 

*/
// CS-SCE CLIENT_MAIN
void* clientMain_sin(void * pData){
	//data initialization
	for (int serverIndex = 0; serverIndex < NUM_SERVERS; serverIndex++){
		for (int o=0;o<numOfDoublePerOperand;o+=1){
			((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[o] = 0;
			((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand1)[o] = 0;
		}
		pSharedMemoryBlock[serverIndex]->inputPart.jobReady = 0;
		((double*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0] = 0;
		pSharedMemoryBlock[serverIndex]->outputPart.resultReady = 0;
	}
	//outer loop
	for (int dispatchTimes=0;dispatchTimes<numOfIteration_sin;dispatchTimes++){
		//inner loop :dispatch jobs
		for (int serverIndex = 0; serverIndex < numOfDispatch_sin; serverIndex++){ //coreId for the serverIndex-th server is (serverIndex+1).
			//spinlock preparation			
			do{
				int locked;
				do{
					locked = pthread_spin_trylock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock); //use trylock to give other threads a chance.
				}while(locked != 0);
				if (pSharedMemoryBlock[serverIndex]->inputPart.jobReady){
					pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock);
				}else{
					break;
				}
			}while(1);
			//start of inputting the serverIndex-th job
			/*two types of transfer
				array transfer
				normal transfer	
				
				template for array transfer
					pSharedMemoryBlock[serverIndex]->inputPart.length = number of parameters transfer during this dispatch
					for (int o=0;o<numOfPara;o++){	
						pointer type((double*) pSharedMemoryBlock[serverIndex]->inputPart.operand0 )[o] operand0 or operand1=\
						x_xxx[outer iteration loop step(dispatchTimes) * numOfPara processed per iteration(numOfParaI)+\
							inner dispatch loop step(serverIndex) * numOfPara processed per dispatch(numOfParaD)+\
							this loop step(o)];		
					}
				template for normal transfer
					((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[0..1..2]=x_sin;
			  two types of division:
				equally distributed	eg. if 300 numOfPara, 100 for each server. usually not choose
				overflow distributed	eg. if 301 numOfPara, 300 for server-0, 1 for server-1
			*/
			((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[0] = input_sin;
			//fixed procedure
			/*
			typedef enum{
				MATRIX_PRODUCT,
				SIN, COS, TAN, COT, ATAN2,
				SQRT, EXP, LOG, ALOG, LOG10, 
				FIND_PRIME_NUMBERS, 
			}ServerFunctionType;
			*/
			pSharedMemoryBlock[serverIndex]->inputPart.functionType = SIN;
			pSharedMemoryBlock[serverIndex]->inputPart.jobReady = 1;
			//unlock spinlock
			pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock);
			
		}
		//inner loop : collect results
		for (int serverIndex = 0; serverIndex < numOfDispatch_sin; serverIndex++){
			do{
				int locked;
				do{
					locked = pthread_spin_trylock(& pSharedMemoryBlock[serverIndex]->outputPart.spinlock); //use trylock to give other threads a chance.
					if (!locked) { int tmp = rand(); }
				}while(locked != 0);
				if (! pSharedMemoryBlock[serverIndex]->outputPart.resultReady){
					pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->outputPart.spinlock);
				}else{
					break;
				}
			}while(1);
			//start of collecting the serverIndex-th result
			/* template for collecting array
				sdot+=((float*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0];	
			*/
			output_sin=((double*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0];
			pSharedMemoryBlock[serverIndex]->outputPart.resultReady = 0;
			pthread_spin_unlock(&pSharedMemoryBlock[serverIndex]->outputPart.spinlock);
		}
	}
	//terminate server thread
	for (int serverIndex = 0; serverIndex < NUM_SERVERS; serverIndex++){
		pSharedMemoryBlock[serverIndex]->outputPart.resultReady = -1;
	}
	
	return NULL;
}
// CS-SCE SERVER_MAIN
void* serverMain_sin(void * pData){
	int serverIndex = *((int*)pData); //coreId for the serverIndex-th server is (serverIndex+1).
	
	do{

		
		do{
			int locked;
			do{
				locked = pthread_spin_trylock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock); //use trylock to give other threads a chance.
				//burn some cpu cycles to yield to the producer.
				if (!locked) { 
					int tmp = rand(); 
					if (pSharedMemoryBlock[serverIndex]->outputPart.resultReady==-1){
						return NULL;
					}	
				}
			}while(locked != 0);
			if (! pSharedMemoryBlock[serverIndex]->inputPart.jobReady){
				pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock);
			}else{
				break;
			}
		}while(1);
		//start of doing job
		do{
			int locked;
			do{
				locked = pthread_spin_trylock(& pSharedMemoryBlock[serverIndex]->outputPart.spinlock); //use trylock to give other threads a chance.
			}while(locked != 0);
			if (pSharedMemoryBlock[serverIndex]->outputPart.resultReady){
				pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->outputPart.spinlock);
			}else{
				break;
			}
		}while(1);
	
		//start of outputting the result at the serverIndex-th server.
		/*
		template for array:
			for (int i =0;i<numOfFloatPerOperand;i++){ 
				((float*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0] +=\
				sin(((float*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[i])*\
				sin(((float*)pSharedMemoryBlock[serverIndex]->inputPart.operand1)[i])*\
			}
		template for normal
			((double*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0]=sin(((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[0])	
		*/
		
		((double*)pSharedMemoryBlock[serverIndex]->outputPart.operandResult)[0]=sin(((double*)pSharedMemoryBlock[serverIndex]->inputPart.operand0)[0]);
		//end of outputting the result
		pSharedMemoryBlock[serverIndex]->outputPart.resultReady = 1;
		pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->outputPart.spinlock);
		//end of doing job
		pSharedMemoryBlock[serverIndex]->inputPart.jobReady = 0;
		pthread_spin_unlock(& pSharedMemoryBlock[serverIndex]->inputPart.spinlock);
	}while(1);
	return NULL;
}
double sce_sin(double x)
{
	//data init
	input_sin = x;
	int rc;
	//parameter variable
	for (int i = 0; i < NUM_SERVERS; i++){
		if (rc = pthread_create(&serverThread[i], &serverThreadAttr[i], serverMain_sin, &serverIndex[i])){
			perror(strerror(rc));
			exit(-1);
		}
	}

	if (rc = pthread_create(&clientThread, &clientThreadAttr, clientMain_sin, NULL)){
		perror(strerror(rc));
		exit(-1);
	}
	
	//joining all threads.
	if (rc = pthread_join(clientThread, NULL)){
		perror(strerror(rc));
		exit(-1);
	}
	
	for (int i = 0; i < NUM_SERVERS; i++){
		if (rc = pthread_join(serverThread[i], NULL)){
			perror(strerror(rc));
			exit(-1);
		}
	}
	
	
	for (int i = 0; i < NUM_SERVERS; i++){
		pthread_spin_destroy(& pSharedMemoryBlock[i]->inputPart.spinlock);
		pthread_spin_destroy(& pSharedMemoryBlock[i]->outputPart.spinlock);
	}

	return output_sin;
}   
