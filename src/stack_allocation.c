#include "scelib.h"
int stack_allocation(){
	int rc;   	
	for (int i = 0; i < NUM_SERVERS; i++){
		//allocating the shared memory
		if (rc = posix_memalign((void**)&pSharedMemoryBlock[i], PAGE_SIZE, sizeof(SharedMemoryBlock))){
			perror(strerror(rc));
			exit(-1);
		}
		pSharedMemoryBlock[i]->inputPart.jobReady = 0;
		pthread_spin_init(& pSharedMemoryBlock[i]->inputPart.spinlock, PTHREAD_PROCESS_PRIVATE);
		pSharedMemoryBlock[i]->outputPart.resultReady = 0;
		pthread_spin_init(& pSharedMemoryBlock[i]->outputPart.spinlock, PTHREAD_PROCESS_PRIVATE);
		if (rc = pthread_attr_init(&serverThreadAttr[i])){
			perror(strerror(rc));
			exit(-1);
		}
		if (rc = pthread_attr_setinheritsched(&serverThreadAttr[i], PTHREAD_EXPLICIT_SCHED)){
			perror(strerror(rc));
			exit(-1);
		}
		if (rc = pthread_attr_setschedpolicy(&serverThreadAttr[i], SCHED_FIFO)){
			perror(strerror(rc));
			exit(-1);
		}
		struct sched_param param;
		param.sched_priority = 50 + i;
		if (rc = pthread_attr_setschedparam(&serverThreadAttr[i], &param)){
			perror(strerror(rc));
			exit(-1);
		}
		cpu_set_t cpuSet;
		CPU_ZERO(&cpuSet);
		CPU_SET((i+1), &cpuSet); //coreId for the (i)th server is (i+1).
		if (rc = pthread_attr_setaffinity_np(&serverThreadAttr[i], sizeof(cpuSet), &cpuSet)){
			perror(strerror(rc));
			exit(-1);
		}
		serverIndex[i] = i; //i's value changes in each iteration, so cannot pass &i into the serverIndex.
	}
	
	//setting the client thread attributes
	if (rc = pthread_attr_init(&clientThreadAttr)){
		perror(strerror(rc));
		exit(-1);
	}
	if (rc = pthread_attr_setinheritsched(&clientThreadAttr, PTHREAD_EXPLICIT_SCHED)){
		perror(strerror(rc));
		exit(-1);
	}
	if (rc = pthread_attr_setschedpolicy(&clientThreadAttr, SCHED_FIFO)){
		perror(strerror(rc));
		exit(-1);
	}
	struct sched_param param;
	param.sched_priority = 99;
	if (rc = pthread_attr_setschedparam(&clientThreadAttr, &param)){
		perror(strerror(rc));
		exit(-1);
	}
	cpu_set_t cpuSet;
	CPU_ZERO(&cpuSet);
	CPU_SET(0, &cpuSet);
	if (rc = pthread_attr_setaffinity_np(&clientThreadAttr, sizeof(cpuSet), &cpuSet)){
		perror(strerror(rc));
		exit(-1);
	}
	return 1;
}