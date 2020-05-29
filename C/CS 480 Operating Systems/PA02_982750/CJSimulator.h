#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "CJConfigParser.h"
#include "CJString.h"
#include "MetaDataParser.h"
#include "simtimer.h"

//Data structures
//Process Control Block structure
typedef struct
   {
      ConfigDataType *pConfigData;
      struct OpCodeType *pMetaData;
      char *processState;
   } ProcessControlBlock;
   
//Process List structure
typedef struct ProcessList
   {
      ProcessControlBlock processBlock;
      
	  struct ProcessList *next;
   } ProcessList;


//Prototyped functions
void runSimulation(ConfigDataType **configData, OpCodeType **metaData);
void runProcess(int processID, int timeRun, OpCodeType *opCode, ConfigDataType *configData, char **tempLog, char *CPUtime);

//FCFS-NP based functions
void handleFCFSNnew(ConfigDataType *configData, OpCodeType *metaData, char **tempLog, char *CPUtime);
void handleFCFSNready( ConfigDataType *configData, ProcessControlBlock *processBk, char **tempLog, char *CPUtime);
void handleFCFSNrun( ConfigDataType *configData, ProcessControlBlock *processBk, char **tempLog, char *CPUtime);

//Helper functions
void updateTSim(char *pStr, char *timeStr, ConfigDataType *configData, char **tempLog, Boolean updateT);
void writeToLog(ConfigDataType *configData, char *tempLog);
int getTotalOpTime(OpCodeType *metaData, ConfigDataType *configData);
int getOpTime(OpCodeType *metaData, ConfigDataType *configData);


#endif //of SIMULATOR_H