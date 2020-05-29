#include "CJSimulator.h"


/* Function: runSimulation
   Input: Assumes it will recieve a valid configuration data and meta data
   Output: Stores updates into a structure
        Either prints these updats as their created and/or sotres them in a log at the end
   Notes:
*/
void runSimulation(ConfigDataType **configData, OpCodeType **metaData)
{
    //copy the data to ensure nothing gets written by accident
    ConfigDataType *configDataCopy = *configData;
    OpCodeType *metaDataCopy = *metaData;
    
    
    //create our temporary structure for holding updates
    char tempLogStr[MAX_STR_LEN];
    char *tempLog = tempLogStr;
    
    //create header
    char header[] = "Simluator Log File \n============================\n";
          
          
    //add header
    concatenateString( tempLog, header );
    
    
    
    //FUTURE TODO create a Process Control Block List
    //ProcessList *procList;
    //procList = (ProcessList *) malloc(sizeof(ProcessList));
    
    //Create our timer
    char *CPUtime;
    CPUtime = (char *)malloc( sizeof(char *) );
    accessTimer(ZERO_TIMER, CPUtime);
    
    //Start the system and update
    updateTSim("OS: System Start", CPUtime, configDataCopy, &tempLog, False);
    updateTSim("OS: Create Process Control Blocks", CPUtime, configDataCopy, &tempLog, True);
    
    //FUTURE TODO create a way to handle multiple processes
    
    //Currently only handling FCFS-N
    switch (configDataCopy->CPUSchCode)
    {
        default:
        handleFCFSNnew(configDataCopy, metaDataCopy, &tempLog, CPUtime);
        break;
    }
   
    //Check if the config is set to log the data
    if(configDataCopy->logTo == LOG_TO_BOTH_CODE
       || configDataCopy->logTo == LOG_TO_FILE_CODE)
       {
           writeToLog(configDataCopy, tempLog);
       }
       
    free(CPUtime);
    
}

//Updates the systems temporary log and if system is set to print will print
    //if the time given has not been initialized it is set to 0
void updateTSim(char *pStr, char *timeStr, ConfigDataType *configData, char **tempLog, Boolean updateT)
{
    int logCode = configData->logTo;
    
    //check to see if the timer needs to be started
    if( updateT == True )
    {
        //The timer is already started so update the time
        accessTimer(LAP_TIMER, timeStr);
    }

    
    char tempTime[getStringLength(timeStr)];
    copyString(tempTime, timeStr);
    
    char tempStr[getStringLength(tempTime)];
    copyString(tempStr, tempTime);
    
    //create the temporary string that will be added to the temp log
   // concatenateString(tempStr, tempTime);
    concatenateString(tempStr, ", ");
    concatenateString(tempStr, pStr);
    concatenateString(tempStr, "\n");
    
    //add string to the temp log
    concatenateString(*tempLog, tempStr);
    
 
    
    
    //check to see if the simulator is supposed to print
    if(logCode == LOG_TO_BOTH_CODE
       || logCode == LOG_TO_MONITOR_CODE)
    {
        //print out the temporary string
        printf(tempStr);
    }

}


void writeToLog(ConfigDataType *configData, char tempLog[])
{
    
    FILE *logFile;
    char *WRITE_ONLY = "w";
           
    //open file
    logFile = fopen("log.txt", WRITE_ONLY);
           
    //print data to file
    fprintf(logFile, tempLog);
           
    //close file
    fclose(logFile);
}

//Handles the "NEW" state changes for FCFS-N specifically
void handleFCFSNnew(ConfigDataType *configData, OpCodeType *metaData, char **tempLog, char *CPUtime)
{
    //create process control blocks
      //FUTURE TODO Handle making multiple process control blocks
    ProcessControlBlock *processBk;
    processBk = (ProcessControlBlock *) malloc(sizeof(ProcessControlBlock*));
    
    //set all processes to the ready state
      //FUTURE TODO Handle multiple processes
    processBk->pConfigData = configData;
    //get the metadata
    processBk->pMetaData = metaData;

    
    processBk->processState = "NEW";
    
    //update the system of the new process blocks
    updateTSim("OS: All Processes Initialized in NEW state", CPUtime, configData, tempLog, True);
    
    handleFCFSNready(configData, processBk, tempLog, CPUtime);
    
    free(processBk);
}

//Handles the "READY" state changes for FCFS-N specifically
void handleFCFSNready( ConfigDataType *configData, ProcessControlBlock *processBk, char **tempLog, char *CPUtime)
{
    //set all processes to the ready state
      //FUTURE TODO Handle multiple processes
    processBk->processState = "READY";
    
    updateTSim("OS: All Processes Initialized in READY state", CPUtime, configData, tempLog, True);
    
    int timeTotal = getTotalOpTime(processBk->pMetaData, configData);
    
    //prep a string to hold the total time
    char printStr[50];
    snprintf(printStr, 50, "OS: Process 0 selected with %d ms time left", timeTotal);
    
    //give an update of the process selected's total time
    updateTSim(printStr, CPUtime, configData, tempLog, True);
    
    //Send the process to the Running state
    handleFCFSNrun( configData, processBk, tempLog, CPUtime);
}

void handleFCFSNrun( ConfigDataType *configData, ProcessControlBlock *processBk, char **tempLog, char *CPUtime)
{
    //set our process to running
    processBk->processState = "RUNNING";
    
    //update the system that the process was set to RUNNING
    updateTSim("OS: Process 0 Initialized in RUNNING state\n", CPUtime, configData, tempLog, True);
    
    //intialize variables for running
    int timeToRun;
    int PROCESS_ID = 0;
    
    //Run the process using the FCFS-N method
    while(processBk->pMetaData != NULL)
    {                  //while there are operation to run...
        //get the time to run the operation
        timeToRun = getOpTime(processBk->pMetaData, configData);
        
        //run the operation
        runProcess(PROCESS_ID, timeToRun, processBk->pMetaData, processBk->pConfigData, tempLog, CPUtime);
        
        //go to next operation
        processBk->pMetaData = processBk->pMetaData->next;
    }
    
    
}

void runProcess(int processID, int timeRun, OpCodeType *opCode, ConfigDataType *configData, char **tempLog, char *CPUtime)
{
    //if given a non running function -> exit
    if(timeRun == 0)
    {
        return;
    }
    
    //prep a string to state the ID
    char printStrStart[50];
    char printStrEnd[50];
    //prep other variables
    char *operationType;
    
    char operationChar = opCode->opLtr;
    
    switch (operationChar)
    {
        case 'P':
        operationType = "operation";
        break;
        
        case 'I':
        operationType = "input";
        break;
        
        case 'O':
        operationType = "output";
        break;
    }
    
    
    //start process
    snprintf(printStrStart, 50, "OS: Process: %d, %s %s start\n", processID, opCode->opName, operationType);
    updateTSim(printStrStart, CPUtime, configData, tempLog, True);
    
    //run process
    runTimer(timeRun);
    
    //update end of process
    snprintf(printStrEnd, 50, "OS: Process: %d, %s %s end\n", processID, opCode->opName, operationType);
    updateTSim(printStrEnd, CPUtime, configData, tempLog, True);
    
}

//Gives an integer that is the sum of the operation times of a given metadata list
//  This is done by recursivly calling down the metadata list structure
int getTotalOpTime(OpCodeType *metaData, ConfigDataType *configData)
{
    //we are NOT the end of the metadata
    if(metaData != NULL)
    {
        int value;
        value = getOpTime(metaData, configData);
        
        return (value + getTotalOpTime(metaData->next, configData));
    }
    
    //we are at the end of the metadata
    return 0;
}

//Gives an integer that is the millisecond time that the first metadata op code must run for
int getOpTime(OpCodeType *metaData, ConfigDataType *configData)
{
    int value;
    value = metaData->opValue * configData->IOCycTime;

    return value;
}


