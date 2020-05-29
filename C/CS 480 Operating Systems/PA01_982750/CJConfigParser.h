//Pre-compiler directive
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

//Headers
#include <stdio.h>
#include <stdlib.h>
#include "CJString.h"

//Data structures

//config data package structure
typedef struct
   {
      char metaFilePath[100];
      int CPUSchCode;
      int logTo;
      char logFilePath[100];
      float version;
      int quantumTime;
      long memoryAvailable;
      int processCycTime;
      int IOCycTime;
   } ConfigDataType;

//Enumerated Data Codes
typedef enum 
   {
      CPU_CODE_FCFS_N,
      CPU_CODE_SJF_N,
      CPU_CODE_FCFS_P,
      CPU_CODE_RR_P,
      CPU_CODE_SRTF_P,
      CPU_CODE_NONE,

      LOG_TO_BOTH_CODE,
      LOG_TO_FILE_CODE,
      LOG_TO_MONITOR_CODE
   } ConfigDataCodes;

//Enumerated Error Codes
    //Begins at 3 to avoid conflict with string err codes
typedef enum 
   {
      CFG_FILE_ACCESS_ERR = 3, 
      CFG_CORRUPT_DESC_ERR,
      CFG_DATA_OUT_OF_RANGE_ERR,
      CFG_CORRUPT_PROMPT_ERR,
      CFG_MISSING_DATA_ERR,

      CFG_VERISON_CODE,
      CFG_META_FILE_NAME_CODE,
      CFG_CPU_SCHED_CODE,
      CFG_Q_CYCLES_CODE,
      CFG_MEM_AVAIL_CODE,
      CFG_PROC_CYCLES_CODE,
      CFG_IO_CYCLES_CODE,
      CFG_LOG_TO_CODE,
      CFG_LOG_FILE_NAME_CODE
   } ConfigCodeMessage;


// prototypes
int processConfig(char* fileName, ConfigDataType **configData);
int getDataLineCode(char *dataBuffer);

char* getErrorCodeMessage(int error);
void printConfigData(ConfigDataType *configData);
void deleteConfig(ConfigDataType **configDataPtr);
Boolean valueInRange(int lineCode,
                     int intVal,
                     double doubleVal,
                     long longVal, 
                     char *stringVal);

ConfigDataCodes getCpuSchedCode( char *codeStr);
ConfigDataCodes getLogToCode( char *codeStr);
void configCodeToString(int code, char *outStr);

#endif //of CONFIG_PARSER_H
