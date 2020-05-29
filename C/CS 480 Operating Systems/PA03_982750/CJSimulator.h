#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "CJConfigParser.h"
#include "CJString.h"
#include "MetaDataParser.h"
#include "simtimer.h"
#include <pthread.h>

//Data structures
//Process Control Block structure
typedef struct ProcessControlBlock
   {
	  int id;
      ConfigDataType *pConfigData;
      struct OpCodeType *pMetaData;
	  struct OpCodeType *currentOp;
	  int totalTime;
	  int * PCBMemory;
	  pthread_mutex_t *lock;
      char *processState;
	  char **tempLogPointer;
	  char **CPUTimePointer;
   } ProcessControlBlock;
   
//Process Linked List structures
//Process Head structure
typedef struct ProcessHead
   {
	  struct ProcessList *processStart;
   } ProcessHead;

//Process List Node structure
typedef struct ProcessList
   {
      struct ProcessControlBlock *processBlock;

	  struct ProcessList *next;
   } ProcessList;
   
typedef enum 
    {
        EMPTY,
        ALLOCATED
    } MemorySpaceVal;


//Prototyped functions
void runSimulation(ConfigDataType **configData, OpCodeType **metaData);
void *runProcess(void *pcbBlock);

//FCFS-NP based functions
void handleFCFSNready( ProcessHead *linkedPCBList );
void *handleNPrun( void * pcb );

//Helper functions
void sendToReady( ProcessHead *linkedPCBList );
void updateTSim(char *printStr, char *timeStr, ConfigDataType *configData, char **tempLog, Boolean updateT);
void writeToLog(ConfigDataType *configData, char tempLog[]);
int getTotalOpTime(OpCodeType *metaData, ConfigDataType *configData);
int getOpTime(OpCodeType *metaData, ConfigDataType *configData);
Boolean accessMemory(ProcessControlBlock *pcbBlk, int allocateMem, int offsetMem);
Boolean allocateMemory(ProcessControlBlock *pcbBlk, int allocateMem, int offsetMem);
void shortestFirstSort(ProcessHead *linkedPCBList);
void swapPCB (ProcessList *linkedPCBListNode1, ProcessList *linkedPCBListNode2);

#endif //of SIMULATOR_H